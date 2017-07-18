using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.IO;
using System;

#if !UNITY_EDITOR   // Works only on Hololens.
using Windows.Networking.Sockets;
using Windows.Networking.Connectivity;
using Windows.Networking;
using Windows.Storage.Streams;
using Windows.Foundation;
using System.Threading.Tasks;
#endif

namespace Gogyo.Network
{
    public class UDP_Receiver : MonoBehaviour
    {
        [SerializeField]
        private int m_localPort = 10386;
#if !UNITY_EDITOR
    private DatagramSocket m_udp;
#else
        private UdpClient m_udp;
        private Thread m_listenThread;
#endif

        private SafeQueue<ReceiveDataType> m_receivedData;

        public delegate void OnReceiveFunc(string data, string address, int port);
        private event OnReceiveFunc m_onReceive;

        public event OnReceiveFunc ReceiveHandler
        {
            add { m_onReceive += value; }
            remove { m_onReceive -= value; }
        }

        // Use this for initialization
#if !UNITY_EDITOR
	async void Start () {
#else
        void Start()
        {
#endif
            m_receivedData = new SafeQueue<ReceiveDataType>();

#if !UNITY_EDITOR
            m_udp = new DatagramSocket();
            m_udp.MessageReceived += Socket_MessageReceived;
            try {
                await m_udp.BindEndpointAsync(null, m_localPort.ToString());
            } catch( IOException e) {
                return;
            }
            //m_udp.Control.DontFragment = true;
            //m_udp.Control.InboundBufferSizeInBytes = 1024 * 1024;
            //m_udp.Control.QualityOfService = SocketQualityOfService.LowLatency;
#else
            m_udp = new UdpClient(m_localPort);
            m_udp.Client.ReceiveTimeout = 1000;
            m_listenThread = new Thread(new ParameterizedThreadStart(ListenThread));
            m_listenThread.Start(this);
#endif
        }

        // Update is called once per frame
        void Update()
        {
            while (m_receivedData.Count > 0)
            {
                ReceiveDataType rcv = m_receivedData.Dequeue();
                //Debug.Log(rcv.data + " from " + rcv.remoteIP.ToString());

                if (null != m_onReceive)
                {
                    m_onReceive.Invoke(rcv.data, rcv.remoteIP.Address.ToString(), rcv.remoteIP.Port);
                }
            }
        }

#if !UNITY_EDITOR
    public async Task Send(string data, string address, int port)
    {
        IOutputStream streamOut = await m_udp.GetOutputStreamAsync(new HostName(address), port.ToString());
        DataWriter writer = new DataWriter(streamOut);
        writer.WriteString(data);

        try
        {
            await writer.StoreAsync();
            //await writer.FlushAsync();
        } catch(Exception e)
        {
            /*
            ReceiveDataType r = new ReceiveDataType();
            r.remoteIP = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 8888);
            r.data = e.Message;
            m_receivedData.Enqueue(r);
            */
        }
    }

#else
        public void Send(string data, string address, int port)
        {
            byte[] b = System.Text.Encoding.ASCII.GetBytes(data);
            m_udp.Send(b, b.Length, address, port);
        }
#endif

#if !UNITY_EDITOR
    private async void Socket_MessageReceived(Windows.Networking.Sockets.DatagramSocket sender,
        Windows.Networking.Sockets.DatagramSocketMessageReceivedEventArgs args)
    {
        // 多重イベントを抑制
        //m_udp.MessageReceived -= Socket_MessageReceived;

        Stream streamIn = args.GetDataStream().AsStreamForRead();
        StreamReader reader = new StreamReader(streamIn);

        int port;
        Int32.TryParse(args.RemotePort, out port);

        ReceiveDataType r = new ReceiveDataType();
        try
        {
            r.data = await reader.ReadLineAsync();
            r.remoteIP = new IPEndPoint(IPAddress.Parse(args.RemoteAddress.ToString()), port);
            m_receivedData.Enqueue(r);
        } catch(Exception e)
        {
        }

        // イベント発生を許可
        //m_udp.MessageReceived += Socket_MessageReceived;
    }
#else

        private static void ListenThread(object callee)
        {
            UDP_Receiver thiz = (UDP_Receiver)callee;
            while (true)
            {
                IPEndPoint remoteIP = null;
                byte[] data;
                if (thiz.m_udp.Available > 0)
                {
                    data = thiz.m_udp.Receive(ref remoteIP);
                    ReceiveDataType r = new ReceiveDataType();
                    r.data = Encoding.ASCII.GetString(data);
                    r.remoteIP = remoteIP;
                    thiz.m_receivedData.Enqueue(r);
                }

                // Sleepしないとフリーズする!?
                Thread.Sleep(1);
            }
        }
#endif

        private struct ReceiveDataType
        {
            public string data;
            public IPEndPoint remoteIP;
        }

        public int Port
        {
            get { return m_localPort; }
        }
        public string LocalAddress
        {
            get
            {
#if !UNITY_EDITOR
                string ip = null;
                foreach (HostName localHostName in NetworkInformation.GetHostNames())
                {
                    if (localHostName.IPInformation != null)
                    {
                        if (localHostName.Type == HostNameType.Ipv4)
                        {
                            ip = localHostName.ToString();
                            break;
                        }
                    }
                }
                return ip;
#else
                return UnityEngine.Network.player.ipAddress;
#endif
            }
        }
    }


    public class SafeQueue<TYPE_NAME> : Queue<TYPE_NAME>
    {
        public SafeQueue() : base()
        {
            m_criticalSection = new object();
        }

        public new void Enqueue(TYPE_NAME item)
        {
            lock (m_criticalSection)
            {
                base.Enqueue(item);
            }
        }

        public new TYPE_NAME Dequeue()
        {
            TYPE_NAME ret = default(TYPE_NAME);
            lock (m_criticalSection)
            {
                ret = base.Dequeue();
            }
            return ret;
        }

        public new int Count
        {
            get
            {
                int ret;
                lock (m_criticalSection)
                {
                    ret = base.Count;
                }
                return ret;
            }
        }

        private object m_criticalSection;
    }
}

