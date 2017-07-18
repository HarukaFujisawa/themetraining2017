using UnityEngine;
using System.Collections.Generic;

namespace Gogyo.Network
{
    /**
     * ネットワークでの通信で発見したデバイス.
     */
    public class PeripheralConnectedDevice
    {
        private string m_address;
        private int m_port;
        private PeripheralManager m_manager = null;
        private PeripheralDevice m_target = null;
        private float m_latestAliveReceive = 0;

        public PeripheralConnectedDevice(string address, int port, PeripheralManager manager)
        {
            m_ability = new List<string>();
            m_port = port;
            m_address = address;
            m_manager = manager;
        }

        public void Send(string data)
        {
            m_manager.UDPSocket.Send(data, m_address, m_port);
        }

        public void OnReceive(string data, string address, int port)
        {
            if (null != m_target)
            {
                m_target.OnReceive(data, address, port);
            }
        }

        public bool IsMatched
        {
            get { return m_target != null; }
        }

        public bool IsAlive
        {
            get
            {
                return (Time.time - m_latestAliveReceive < 12.0f) ? true : false;
            }
        }

        public void Match(PeripheralDevice target)
        {
            m_target = target;
            m_latestAliveReceive = Time.time;
        }

        public void UnMatch()
        {
            m_target = null;
        }

        public bool IsSame(string address, int port)
        {
            if (address == m_address && port == m_port)
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
            Send("{\"pcmd\":\"bye\"}");
        }

        public void SendAlive()
        {
            Send("{\"pcmd\":\"alive\"}");
            CheckAlive();   // aliveを送ると同時にaliveを確認する.
        }

        private void CheckAlive()
        {
            if(!IsAlive)
            {
                SendBye();
                m_manager.ProcessBye(this);
            }
        }

        public void ReceiveAlive()
        {
            m_latestAliveReceive = Time.time;
        }

        private List<string> m_ability;
    }
}
