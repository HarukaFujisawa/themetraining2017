using UnityEngine;
using System.Collections.Generic;

namespace Gogyo.Network
{
    [System.Serializable]
    public abstract class PeripheralDevice : MonoBehaviour
    {
        [SerializeField]
        public PeripheralManager m_manager = null;
        [SerializeField]
        private List<PeripheralConnectedDevice> m_target;

        protected virtual void Start()
        {
            m_manager.Register(this);
        }

        protected virtual void OnDisable()
        {
            m_manager.UnRegister(this);
        }

        public PeripheralDevice()
        {
            m_requireAbility = new List<string>();
            m_capableAbility = new List<string>();
            m_target = new List<PeripheralConnectedDevice>();
        }

        public abstract void OnReceive(string data, string address, int port);

        public virtual void OnConnected(PeripheralConnectedDevice d)
        {
        }

        public virtual void OnDisconnected(PeripheralConnectedDevice d)
        {
        }

        public virtual void OnRequested()
        {
        }

        public void Send(string data)
        {
            foreach(PeripheralConnectedDevice d in m_target)
            {
                d.Send(data);
            }
        }

        public virtual bool IsAcceptable(PeripheralConnectedDevice cd)
        {
            bool ret = false;
            if(cd.IsCapable(m_requireAbility))
            {   // 登録されているすべてのabilityを満たすものを受理する.
                ret = true;
            }
            return ret;
        }

        public bool IsMatched
        {
            get { return m_target.Count > 0; }
        }

        public void Match(PeripheralConnectedDevice connectedDevice)
        {
            if(m_target.Count == 0)
            {
                InvokeRepeating("SendAlive", 3.0f, 3.0f);
            }

            m_target.Add(connectedDevice);
            OnConnected(connectedDevice);
        }

        public void UnMatch(PeripheralConnectedDevice connectedDevice)
        {
            OnDisconnected(connectedDevice);
            m_target.Remove(connectedDevice);

            if (m_target.Count == 0)
            {
                CancelInvoke("SendAlive");
            }
        }

        private void SendAlive()
        {
            foreach(PeripheralConnectedDevice d in m_target)
            {
                d.SendAlive();
            }
        }

        public bool IsAlreadyConnected(PeripheralConnectedDevice connectedDevice, int devidx)
        {
            bool ret = false;
            foreach(PeripheralConnectedDevice d in m_target)
            {
                if(d.IsSame(connectedDevice, devidx))
                {
                    ret = true;
                    break;
                }
            }
            return ret;
        }

        public List<PeripheralConnectedDevice> Target
        {
            get { return m_target; }
        }

        public List<string> RequireAbility
        {
            get { return m_requireAbility; }
        }
        public List<string> CapableAbility
        {
            get { return m_capableAbility; }
        }

        [SerializeField]
        private List<string> m_requireAbility;
        [SerializeField]
        private List<string> m_capableAbility;
    }
}
