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
        private List<PeripheralConnectedDevice> m_connectedDevices;
        private List<PeripheralDevice> m_registeredDevices;
        private List<string> m_ability;

        const string BROADCAST_ADDRESS = "255.255.255.255";

        public PeripheralManager()
        {
            m_connectedDevices = new List<PeripheralConnectedDevice>();
            m_registeredDevices = new List<PeripheralDevice>();
            m_ability = new List<string>();

            m_ability.Add("hololens");
        }

        // Use this for initialization
        void Start()
        {
            m_udp = GetComponent<UDP_Receiver>();
            m_udp.ReceiveHandler += OnReceive;
            InvokeRepeating("SendSearch", 1.0f, m_searchRate);
        }

        // Update is called once per frame
        void Update()
        {
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

                PeripheralConnectedDevice device = FindByAddress(address, port);

                PeripheralProtocol prtcl = JsonUtility.FromJson<PeripheralProtocol>(data);
                switch (prtcl.pcmd)
                {
                    case "search":
                        SendAvailable(address, port);
                        break;
                    case "available":
                        {
                            PeripheralAvailable p = JsonUtility.FromJson<PeripheralAvailable>(data);
                            if (null == device)
                            {   // 新規のデバイス.
                                device = new PeripheralConnectedDevice(address, port, this);
                                foreach (string a in p.ability)
                                {
                                    device.Ability.Add(a);
                                }
                                m_connectedDevices.Add(device);

                                if (TryMatching(device))
                                {
                                    // 対応するデバイスみつかった.
                                    SendRequest(address, port);
                                }
                            }
                            else
                            {
                                // 既に知ってるデバイスは無視する.
                            }
                        }
                        break;
                    case "request":
                        if(null != device)
                        {
                            TryMatching(device);

                            if(null != device.Target)
                            {
                                device.Target.OnRequested();
                            }
                        }
                        break;
                    case "alive":
                        if(null != device)
                        {
                            device.ReceiveAlive();
                        }
                        break;
                    case "bye":
                        {
                            if (null != device)
                            {
                                ProcessBye(device);
                            }
                        }
                        break;

                    default:
                        // PeripheralDeviceに処理を委譲.
                        if (null != device)
                        {
                            device.OnReceive(data, address, port);
                        }
                        break;
                }
            }
        }

        public PeripheralConnectedDevice FindByAddress(string address, int port)
        {
            PeripheralConnectedDevice ret = null;
            foreach (PeripheralConnectedDevice d in m_connectedDevices)
            {
                if (d.IsSame(address, port))
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
        }

        public void UnRegister(PeripheralDevice device)
        {
            m_registeredDevices.Remove(device);
        }

        public bool TryMatching(PeripheralConnectedDevice device)
        {
            bool ret = false;
            foreach (PeripheralDevice d in m_registeredDevices)
            {
                if (!d.IsMatched && !device.IsMatched && d.IsAcceptable(device))
                {
                    device.Match(d);
                    d.Match(device);
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
            PeripheralAvailable p = new PeripheralAvailable();
            p.pcmd = "available";
            p.ability = new string[m_ability.Count];
            int i = 0;
            foreach(string a in m_ability)
            {
                p.ability[i] = a;
                i++;
            }
            m_udp.Send(JsonUtility.ToJson(p), address, port);
        }

        private void SendRequest(string address, int port)
        {
            m_udp.Send("{\"pcmd\":\"request\"}", address, port);
        }

        public void ProcessBye(PeripheralConnectedDevice device)
        {
            device.Target.UnMatch();
            device.UnMatch();
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
        public class PeripheralAvailable : PeripheralProtocol
        {
            public string[] ability;
        }

    }
}

