using UnityEngine;
using Gogyo.Network;


public class RemoteMotionSensor : PeripheralDevice
{
    public ParticleSystem particle;

    public GameObject cube;

    public GameObject angry_mayuge;
    public GameObject sad_mayuge;


    private Material cubeRenderer;
    private Color default_cubeColor;
    public float acc_thresh = 0.1f;
    private Vector3 acc_prev;

    protected override void Start()
    {
        Ability.Add("motion");
        base.Start();

        cubeRenderer = cube.GetComponent<Renderer>().material;
        default_cubeColor = cubeRenderer.color;
    }

    private void Update()
    {
        //print(transform.rotation);
        if(Input.GetKeyUp(KeyCode.P))
        {
            //パーティクル噴出
            particle.Play();
            cubeRenderer.color = Color.red;

            //眉毛かえる
            angry_mayuge.SetActive(!angry_mayuge.activeSelf);
            sad_mayuge.SetActive(!sad_mayuge.activeSelf);

        }
        else
        {
            if(cubeRenderer.color != default_cubeColor)
            {
                cubeRenderer.color = default_cubeColor;
            }
        }
    }


    public override void OnReceive(string data, string address, int port)
    {
        var sensor = JsonUtility.FromJson<SensorProtocol>(data);

        Vector3 v = new Vector3(sensor.acc.x, sensor.acc.y, sensor.acc.z);
        v.Normalize();

        // 右手系 → 左手系変換して代入
        //transform.rotation = new Quaternion(-sensor.rot.x, sensor.rot.y, sensor.rot.z, -sensor.rot.w);
        transform.rotation = new Quaternion(-sensor.rot.x, sensor.rot.y, -sensor.rot.z, sensor.rot.w);
        //transform.rotation = new Quaternion(sensor.rot.x, sensor.rot.y, sensor.rot.z, sensor.rot.w);

        //加速度が閾値以上来たらパーティクル出すとかしてみる//
        Debug.Log(v);
        if (acc_prev == Vector3.zero)
        {
            acc_prev = v;
        }
        else
        {
            Vector3 acc_diff = v - acc_prev;
            Material cubeRenderer = cube.GetComponent<Renderer>().material;
            if (Mathf.Abs(acc_diff.x) >= acc_thresh ||
                Mathf.Abs(acc_diff.y) >= acc_thresh ||
                Mathf.Abs(acc_diff.z) >= acc_thresh)
            {
                cubeRenderer.color = Color.red;
                //パーティクル噴出
                particle.Play();

                //眉毛かえる
                angry_mayuge.SetActive(!angry_mayuge.activeSelf);
                sad_mayuge.SetActive(!sad_mayuge.activeSelf);

            }
            else
            {
                cubeRenderer.color = default_cubeColor;
            }
        }
        acc_prev = v;
    }

    [System.Serializable]
    public struct GyroProtocol
    {
        public float x;
        public float y;
        public float z;
    }

    [System.Serializable]
    public struct AccProtocol
    {
        public float x;
        public float y;
        public float z;
    }

    [System.Serializable]
    public struct PostureRotProtocol
    {
        public float x;
        public float y;
        public float z;
        public float w;
    }

    [System.Serializable]
    public class SensorProtocol
    {
        public int time;
        [SerializeField]
        public GyroProtocol gyro;
        [SerializeField]
        public AccProtocol acc;
        [SerializeField]
        public PostureRotProtocol rot;
    }

}

