using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Net;

namespace Gogyo.Network
{
    [RequireComponent(typeof(UDP_Receiver))]
    public class PeripheralManager : MonoBehaviour
    {
        private UDP_Receiver m_udp;
        private float m_searchRate = 10.0f;
        [SerializeField]
        private List<PeripheralConnectedDevice> m_connectedDevices;
        [SerializeField]
        private List<PeripheralDevice> m_registeredDevices;

        const string BROADCAST_ADDRESS = "255.255.255.255";

        public PeripheralManager()
        {
            m_connectedDevices = new List<PeripheralConnectedDevice>();
            m_registeredDevices = new List<PeripheralDevice>();
        }

        // Use this for initialization
        void Start()
        {
            m_udp = GetComponent<UDP_Receiver>();
            m_udp.ReceiveHandler += OnReceive;
            InvokeRepeating("SendSearch", 1.0f, m_searchRate);
        }

        private float m_latestAliveCheck = 0;
        // Update is called once per frame
        void Update()
        {

            // 定期的にaliveをチェックする.
            if(Time.time - m_latestAliveCheck > 3.0f)
            {
                for(int i = m_connectedDevices.Count - 1; i >= 0; i--)
                {   // 削除するのでforeachは使えない
                    m_connectedDevices[i].CheckAlive();
                }
                m_latestAliveCheck = Time.time;
            }
        }

        void OnReceive(string data, string address, int port)
        {
            if (!IsSelfPacket(address, port))   // 自分が送ったパケットは受けない
            {
                TextMesh text = GetComponentInChildren<TextMesh>();
                if (null != text)
                {
                    text.text = data + " from " + address + ":" + port;
                }

                PeripheralProtocol prtcl = JsonUtility.FromJson<PeripheralProtocol>(data);
                int devidx = GetDeviceIdxFromJson(data);
                //Debug.Log(data + " from " + address + ":" + port + " --- devidx: " + devidx);
                PeripheralConnectedDevice device = null;
                {
                    // 接続しているPeripheralConnectedDeviceを取得する
                    device = FindConnectedDevice(address, port, devidx);
                }

                switch (prtcl.pcmd)
                {
                    case "search":
                        {   // {"pcmd":"search"}
                            // 機器探索のためのトリガ。broadcastアドレスに送信する。
                            SendAvailable(address, port);
                            Debug.Log("[GogyoNetwork]: Received search pcmd from " + address + ":" + port);
                        }
                        break;
                    case "available":
                        {   // {"pcmd":"available","devidx":N,"ability":["ability_1","ability_2",...]}
                            //  機器探索への応答をPeripheralDevice単位で行い，同時にPeripheralDeviceのアビリティの通達を行う。
                            PeripheralAvailable p = JsonUtility.FromJson<PeripheralAvailable>(data);
                            if(!data.Contains("\"devidx\""))
                            {   // HololensのJsonUtilityが、メンバーがいないときに0を代入してしまう問題への対策.
                                p.devidx = -1;
                            }

                            if (null == device)
                            {
                                device = new PeripheralConnectedDevice(address, port, this, p.ability);
                                foreach (string a in p.ability)
                                {
                                    device.Ability.Add(a);
                                }

                                Debug.Log("[GogyoNetwork]: Received available pcmd from " + address + ":" + port + ", data: " + data);

                                if (TryMatching(device, p.devidx))
                                {
                                    // 対応するデバイスみつかった.
                                    SendRequest(address, port, p.devidx, GetDeviceIndex(device.Target));
                                    m_connectedDevices.Add(device);
                                }
                            }
                            else
                            {
                                // 既に知ってるデバイスは無視する.
                            }
                        }
                        break;
                    case "request":
                        {   // {"pcmd":"request","devidx":N,"selfdevidx":M}
                            // 接続要求の発行。発行元は接続を確立。devidxは要求先のデバイスインデックス。selfdevidxは要求元のデバイスインデックス。
                            PeripheralRequestProtocol p = JsonUtility.FromJson<PeripheralRequestProtocol>(data);
                            if(!data.Contains("\"devidx\""))
                            {   // HololensのJsonUtilityが、メンバーがいないときに0を代入してしまう問題への対策.
                                p.devidx = -1;
                            }
                            Debug.Log("[GogyoNetwork]: Received request pcmd from " + address + ":" + port + ", devidx: " + p.devidx + ", selfdevidx: " + p.selfdevidx );

                            if(p.devidx < m_registeredDevices.Count)
                            {
                                PeripheralConnectedDevice d = new PeripheralConnectedDevice(address, port, this);

                                if (ForceMatching(d, p.devidx, p.selfdevidx))
                                {
                                    m_connectedDevices.Add(d);
                                    if (null != d.Target)
                                    {
                                        d.Target.OnRequested();
                                    }
                                }
                            }
                        }
                        break;
                    case "alive":
                        if(null != device)
                        {   //  {"pcmd":"request","devidx":N}
                            //  生存確認用の通達。
                            Debug.Log("[GogyoNetwork]: Received alive pcmd from " + address + ":" + port + ": " + device.TargetDeviceIndex);
                            device.ReceiveAlive();
                        }
                        break;
                    case "bye":
                        {   //  {"pcmd":"bye","devidx":N}
                            //  一定時間生存確認が無い場合に通達して以後切断。
                            if (null != device)
                            {
                                Debug.Log("[GogyoNetwork]: Received bye pcmd from " + address + ":" + port + ": " + device.TargetDeviceIndex);
                                ProcessBye(device);
                            }
                        }
                        break;

                    default:
                        // PeripheralDeviceに処理を委譲.
                        if (null != device && null != device.Target)
                        {
                            device.Target.OnReceive(data, address, port);
                        } else
                        {
                            // devidxが書かれていない通信内容は同じデバイスを登録している全員に伝える.
                            List<PeripheralConnectedDevice> devices = FindByAddress(address, port);
                            foreach(PeripheralConnectedDevice d in devices)
                            {
                                if (null != d.Target)
                                {
                                    d.Target.OnReceive(data, address, port);
                                }
                            }
                        }
                        break;
                }
            }
        }

        private int GetDeviceIndex(PeripheralDevice dev)
        {
            int ret = 0;
            for(int i = 0; i < m_registeredDevices.Count; i++)
            {
                if(m_registeredDevices[i] == dev)
                {
                    ret = i;
                    break;
                }
            }
            return ret;
        }

        public List<PeripheralConnectedDevice> FindByAddress(string address, int port)
        {
            List<PeripheralConnectedDevice> ret = new List<PeripheralConnectedDevice>();
            foreach (PeripheralConnectedDevice d in m_connectedDevices)
            {
                if (d.IsSame(address, port))
                {
                    ret.Add(d);
                }
            }
            return ret;
        }

        public PeripheralConnectedDevice FindConnectedDevice(string address, int port, int session)
        {
            PeripheralConnectedDevice ret = null;
            if (session >= 0 && session < m_registeredDevices.Count)
            {
                foreach (PeripheralConnectedDevice d in m_registeredDevices[session].Target)
                {
                    if (d.IsSame(address, port, d.TargetDeviceIndex))
                    {
                        ret = d;
                        break;
                    }
                }
            }
            else
            {
                foreach(PeripheralDevice r in m_registeredDevices)
                {
                    foreach(PeripheralConnectedDevice d in r.Target)
                    {
                        if(d.IsSame(address, port))
                        {
                            return d;
                        }

                    }
                }
            }
            return ret;
        }

        public void Register(PeripheralDevice device)
        {
            m_registeredDevices.Add(device);
            int i = 0;
            foreach(PeripheralConnectedDevice d in m_connectedDevices)
            {   // ちょっと非効率.
                TryMatching(d, i);
                i++;
            }
        }

        public void UnRegister(PeripheralDevice device)
        {
            m_registeredDevices.Remove(device);
        }

        // アビリティに関係なくPeripheralDeviceとPeripheralConnectedDeviceのマッチングをとる
        public bool ForceMatching(PeripheralConnectedDevice connectedDevice, int regDeviceIdx, int cnctDeviceIdx)
        {
            bool ret = false;

            if (regDeviceIdx >= 0 && regDeviceIdx < m_registeredDevices.Count)
            {
                PeripheralDevice registeredDevice = m_registeredDevices[regDeviceIdx];
                if (!registeredDevice.IsAlreadyConnected(connectedDevice, cnctDeviceIdx))  // 同じコネクションを２つ張らない
                {
                    if (!connectedDevice.IsMatched)
                    {
                        connectedDevice.Match(registeredDevice, cnctDeviceIdx);
                        registeredDevice.Match(connectedDevice);
                        ret = true;
                    }
                }
            }
            else
            {
                foreach (PeripheralDevice r in m_registeredDevices)
                {
                    if (!r.IsAlreadyConnected(connectedDevice, cnctDeviceIdx))  // 同じコネクションを２つ張らない
                    {
                        if (!connectedDevice.IsMatched)
                        {
                            connectedDevice.Match(r, cnctDeviceIdx);
                            r.Match(connectedDevice);
                            ret = true;
                            break;
                        }
                    }
                }
            }

            return ret;
        }

        // アビリティを加味してPeripheralDeviceとPeripheralConnectedDeviceのマッチングをとる
        public bool TryMatching(PeripheralConnectedDevice connectedDevice, int regDeviceIdx, int cnctDeviceIdx)
        {
            bool ret = false;

            if (regDeviceIdx >= 0 && regDeviceIdx < m_registeredDevices.Count)
            {
                PeripheralDevice registeredDevice = m_registeredDevices[regDeviceIdx];
                if (!registeredDevice.IsAlreadyConnected(connectedDevice, cnctDeviceIdx))  // 同じコネクションを２つ張らない
                {
                    if (!connectedDevice.IsMatched && registeredDevice.IsAcceptable(connectedDevice))
                    {
                        connectedDevice.Match(registeredDevice, cnctDeviceIdx);
                        registeredDevice.Match(connectedDevice);
                        ret = true;
                    }
                }
            }
            else
            {
                foreach (PeripheralDevice r in m_registeredDevices)
                {
                    if (!r.IsAlreadyConnected(connectedDevice, cnctDeviceIdx))  // 同じコネクションを２つ張らない
                    {
                        if (!connectedDevice.IsMatched && r.IsAcceptable(connectedDevice))
                        {
                            connectedDevice.Match(r, cnctDeviceIdx);
                            r.Match(connectedDevice);
                            ret = true;
                            break;
                        }
                    }
                }
            }

            return ret;
        }

        public bool TryMatching(PeripheralConnectedDevice device, int cnctDeviceIdx)
        {
            bool ret = false;
            for(int i = 0; i < m_registeredDevices.Count; i++)
            {
                if (TryMatching(device, i, cnctDeviceIdx))
                {
                    ret = true;
                    break;
                }
            }
            return ret;
        }

        public bool IsSelfPacket(string address, int port)
        {
            if(address == UDPSocket.LocalAddress && port == UDPSocket.Port)
            {
                return true;
            }
            return false;
        }

        public UDP_Receiver UDPSocket
        {
            get { return m_udp; }
        }

        private void SendSearch()
        {
            m_udp.Send("{\"pcmd\":\"search\"}", BROADCAST_ADDRESS, m_udp.Port);
        }

        private void SendAvailable(string address, int port)
        {
            int idx = 0;
            foreach (PeripheralDevice d in m_registeredDevices)
            {
                PeripheralAvailable p = new PeripheralAvailable();
                p.pcmd    = "available";
                p.ability = new string[d.CapableAbility.Count];
                p.devidx  = idx;
                int i = 0;
                foreach (string a in d.CapableAbility)
                {
                    p.ability[i] = a;
                    i++;
                }
                m_udp.Send(JsonUtility.ToJson(p), address, port);

                idx++;
            }
        }

        private void SendRequest(string address, int port, int deviceIdx, int selfDeviceIdx)
        {
            PeripheralRequestProtocol p = new PeripheralRequestProtocol();
            p.pcmd = "request";
            p.devidx = deviceIdx;
            p.selfdevidx = selfDeviceIdx;
            m_udp.Send(JsonUtility.ToJson(p), address, port);
        }

        private int GetDeviceIdxFromJson(string data)
        {
            PeripheralProtocolEx p = new PeripheralProtocolEx();
            if (data.Contains("\"devidx\""))
            {   // HololensのJsonUtilityが、メンバーがいないときに0を代入してしまう問題への対策.
                p = JsonUtility.FromJson<PeripheralProtocolEx>(data);
                if (null != p)
                {
                    return p.devidx;
                }
            }
            return -1;
        }

        public void ProcessBye(PeripheralConnectedDevice device)
        {
            if (null != device.Target)
            {
                device.Target.UnMatch(device);
                device.UnMatch();
            }
            m_connectedDevices.Remove(device);
        }

        //  Class for JSON encode/decode.
        [System.Serializable]
        public class PeripheralProtocol
        {
            public string pcmd;
        }

        //  Class for JSON encode/decode.
        [System.Serializable]
        public class PeripheralProtocolEx : PeripheralProtocol
        {
            public int devidx = -1;
        }

        //  Class for JSON encode/decode.
        [System.Serializable]
        public class PeripheralRequestProtocol : PeripheralProtocolEx
        {
            public int selfdevidx;
        }

        //  Class for JSON encode/decode.
        [System.Serializable]
        public class PeripheralAvailable : PeripheralProtocolEx
        {
            public string[] ability;
        }
    }
}

