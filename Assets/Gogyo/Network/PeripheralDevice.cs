using UnityEngine;
using System.Collections.Generic;

namespace Gogyo.Network
{
    public abstract class PeripheralDevice : MonoBehaviour
    {
        //[SerializeField]
        //public PeripheralManager m_manager = null;

        private PeripheralConnectedDevice m_target = null;

        protected virtual void Start()
        {
            //m_manager.Register(this);
            PeripheralManager.instance.Register(this);
        }

        protected virtual void OnDisable()
        {
            //m_manager.UnRegister(this);
            PeripheralManager.instance.UnRegister(this);
        }

        public PeripheralDevice()
        {
            m_ability = new List<string>();
        }

        public abstract void OnReceive(string data, string address, int port);

        public virtual void OnConnected()
        {   // 単純にリクエストしてみる.
            Send("{\"pcmd\":\"request\"}");
        }

        public virtual void OnDisconnected()
        {
        }

        public virtual void OnRequested()
        {
        }

        public void Send(string data)
        {
            if (null != m_target)
            {
                m_target.Send(data);
            }
        }

        public virtual bool IsAcceptable(PeripheralConnectedDevice cd)
        {
            bool ret = false;
            if(cd.IsCapable(m_ability))
            {   // 登録されているすべてのabilityを満たすものを受理する.
                ret = true;
            }
            return ret;
        }

        public bool IsMatched
        {
            get { return m_target != null; }
        }

        public void Match(PeripheralConnectedDevice target)
        {
            m_target = target;
            OnConnected();

            InvokeRepeating("SendAlive", 5.0f, 5.0f);
        }

        public void UnMatch()
        {
            m_target = null;
            OnDisconnected();

            CancelInvoke("SendAlive");
        }

        private void SendAlive()
        {
            if(null != m_target)
            {
                m_target.SendAlive();
            }
        }

        public PeripheralConnectedDevice Target
        {
            get { return m_target; }
        }

        public List<string> Ability
        {
            get { return m_ability; }
        }

        private List<string> m_ability;
    }
}
