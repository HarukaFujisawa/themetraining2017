using UnityEngine;
using System.Collections.Generic;

namespace Gogyo.Network
{
    /**
     * ネットワークでの通信で発見したデバイス.
     */
    [System.Serializable]
    public class PeripheralConnectedDevice
    {
        [SerializeField]
        private string m_address;
        [SerializeField]
        private int m_port;
        [SerializeField]
        private int m_devidx;
        private PeripheralManager m_manager = null;
        [SerializeField]
        private PeripheralDevice m_target = null;
        [SerializeField]
        private float m_latestAliveReceive = 0;
        [SerializeField]
        private List<string> m_ability;

        public PeripheralConnectedDevice(string address, int port, PeripheralManager manager, string[] abilities = null)
        {
            m_ability = new List<string>();
            if (null != abilities)
            {
                for(int i = 0; i < abilities.Length; i++)
                {
                    m_ability.Add(abilities[i]);
                }
            }
            m_port = port;
            m_address = address;
            m_manager = manager;
            m_devidx = -1;
            m_latestAliveReceive = Time.time;
        }

        public void Send(string data)
        {
            m_manager.UDPSocket.Send(data, m_address, m_port);
        }

        public bool IsMatched
        {
            get { return m_target != null; }
        }

        public bool IsAlive
        {
            get
            {
                return (Time.time - m_latestAliveReceive < 50.0f) ? true : false;
            }
        }

        public void Match(PeripheralDevice target, int deviceIdx)
        {
            m_target = target;
            m_devidx = deviceIdx;
            m_latestAliveReceive = Time.time;
        }

        public void UnMatch()
        {
            m_target = null;
        }

        public bool IsSame(string address, int port, int devidx = -1)
        {
            if (devidx < 0)
            {
                if (address == m_address && port == m_port)
                {
                    return true;
                }
            }
            else
            {
                if (address == m_address && port == m_port && devidx == m_devidx)
                {
                    return true;
                }
            }
            return false;
        }

        public bool IsSame(PeripheralConnectedDevice connectedDevice, int devidx)
        {
            if (connectedDevice.m_address == m_address && connectedDevice.m_port == m_port && devidx == m_devidx)
            {
                return true;
            }
            return false;
        }

    public bool IsCapable(string ability)
        {
            bool ret = false;
            foreach (string a in m_ability)
            {
                if (a == ability)
                {
                    ret = true;
                    break;
                }
            }
            return ret;
        }

        public bool IsCapable(List<string> ability)
        {
            if (m_ability.Count > 0 && ability.Count > 0)
            {
                bool ret = true;
                foreach (string a in ability)
                {
                    if (!IsCapable(a))
                    {
                        ret = false;
                        break;
                    }
                }
                return ret;
            }
            return false;
        }

        public int Port
        {
            get { return m_port; }
            set { m_port = value; }
        }

        public string Address
        {
            get { return m_address; }
            set { m_address = value; }
        }

        public List<string> Ability
        {
            get { return m_ability; }
        }

        public PeripheralDevice Target
        {
            get { return m_target; }
        }

        public void SendBye()
        {
            Send("{\"pcmd\":\"bye\",\"devidx\":" + m_devidx + "}");
            m_devidx = -1;
        }

        public void SendAlive()
        {
            Send("{\"pcmd\":\"alive\",\"devidx\":" + m_devidx + "}");
        }

        public void CheckAlive()
        {
            if(!IsAlive)
            {
                if (IsMatched)
                {
                    SendBye();
                }
                m_manager.ProcessBye(this);
            }
        }

        public void ReceiveAlive()
        {
            m_latestAliveReceive = Time.time;
        }

        public int TargetDeviceIndex
        {
            get { return m_devidx; }
        }
    }
}
