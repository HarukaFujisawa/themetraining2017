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
                PeripheralConnectedDevice device = null;
                {
                    // 接続しているPeripheralConnectedDeviceを取得する
                    device = FindConnectedDevice(address, port, devidx);
                }

                switch (prtcl.pcmd)
                {
                    case "search":
                        {
                            SendAvailable(address, port);
                            Debug.Log("[GogyoNetwork]: Received search pcmd from " + address + ":" + port);
                        }
                        break;
                    case "available":
                        {
                            PeripheralAvailable p = JsonUtility.FromJson<PeripheralAvailable>(data);
                            if (null == device)
                            {
                                device = new PeripheralConnectedDevice(address, port, this);
                                foreach (string a in p.ability)
                                {
                                    device.Ability.Add(a);
                                }

                                Debug.Log("[GogyoNetwork]: Received available pcmd from " + address + ":" + port + ", data: " + data);

                                if (TryMatching(device))
                                {
                                    // 対応するデバイスみつかった.
                                    SendRequest(address, port, p.devidx);
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
                        {
                            PeripheralProtocolEx p = JsonUtility.FromJson<PeripheralProtocolEx>(data);
                            Debug.Log("[GogyoNetwork]: Received request pcmd from " + address + ":" + port + ", DeviceIdx: " + p.devidx );

                            if(p.devidx < m_registeredDevices.Count)
                            {
                                PeripheralConnectedDevice d = new PeripheralConnectedDevice(address, port, this);

                                if (TryMatching(d, p.devidx))
                                {
                                    m_connectedDevices.Add(d);
                                    if (null != device.Target)
                                    {
                                        device.Target.OnRequested();
                                    }
                                }
                            }
                        }
                        break;
                    case "alive":
                        if(null != device)
                        {
                            Debug.Log("[GogyoNetwork]: Received alive pcmd from " + address + ":" + port + ": " + device.TargetDeviceIndex);
                            device.ReceiveAlive();
                        }
                        break;
                    case "bye":
                        {
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
            foreach (PeripheralConnectedDevice d in m_connectedDevices)
            {
                if (d.IsSame(address, port, session))
                {
                    ret = d;
                    break;
                }
            }
            return ret;
        }

        public void Register(PeripheralDevice device)
        {
            m_registeredDevices.Add(device);
            foreach(PeripheralConnectedDevice d in m_connectedDevices)
            {   // ちょっと非効率.
                TryMatching(d);
            }
        }

        public void UnRegister(PeripheralDevice device)
        {
            m_registeredDevices.Remove(device);
        }

        public bool TryMatching(PeripheralConnectedDevice connectedDevice, int deviceIdx)
        {
            bool ret = false;

            if (deviceIdx >= 0 && deviceIdx < m_registeredDevices.Count)
            {
                PeripheralDevice registeredDevice = m_registeredDevices[deviceIdx];
                if (!registeredDevice.IsAlreadyConnected(connectedDevice))  // 同じコネクションを２つ張らない
                {
                    if (!connectedDevice.IsMatched && registeredDevice.IsAcceptable(connectedDevice))
                    {
                        connectedDevice.Match(registeredDevice, deviceIdx);
                        registeredDevice.Match(connectedDevice);
                        ret = true;
                    }
                }
            }
            return ret;
        }

        public bool TryMatching(PeripheralConnectedDevice device)
        {
            bool ret = false;
            for(int i = 0; i < m_registeredDevices.Count; i++)
            {
                if (TryMatching(device, i))
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

        private void SendRequest(string address, int port, int deviceIdx)
        {
            PeripheralProtocolEx p = new PeripheralProtocolEx();
            p.pcmd = "request";
            p.devidx = deviceIdx;
            m_udp.Send(JsonUtility.ToJson(p), address, port);
        }

        private int GetDeviceIdxFromJson(string data)
        {
            PeripheralProtocolEx p = new PeripheralProtocolEx();
            p.devidx = -1;
            p = JsonUtility.FromJson<PeripheralProtocolEx>(data);
            if(null != p)
            {
                return p.devidx;
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
            public int devidx;
        }

        //  Class for JSON encode/decode.
        [System.Serializable]
        public class PeripheralAvailable : PeripheralProtocolEx
        {
            public string[] ability;
        }
    }
}

