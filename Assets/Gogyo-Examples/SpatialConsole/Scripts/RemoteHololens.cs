using UnityEngine;
using Gogyo.Network;

public class RemoteHololens : PeripheralDevice
{
    protected override void Start()
    {
        //Ability.Add("hololens");
        base.Start();
    }

    private void Update()
    {
        RemoteHololensProtocol sendData = new RemoteHololensProtocol();
        sendData.pos = Camera.main.transform.position;
        sendData.quat = Camera.main.transform.rotation;
        string data = JsonUtility.ToJson(sendData);
        base.Send(data);
    }

    public override void OnReceive(string data, string address, int port)
    {
        RemoteHololensProtocol receiveData = JsonUtility.FromJson<RemoteHololensProtocol>(data);
        transform.position = receiveData.pos;
        transform.rotation = receiveData.quat;
    }

    [System.Serializable]
    public class RemoteHololensProtocol
    {
        [SerializeField]
        public Vector3 pos;
        [SerializeField]
        public Quaternion quat;
    }
}


