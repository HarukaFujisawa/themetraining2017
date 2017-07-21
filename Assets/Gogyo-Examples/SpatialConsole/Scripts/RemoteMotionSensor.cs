using UnityEngine;
using Gogyo.Network;

using System.IO;

public class RemoteMotionSensor : PeripheralDevice
{

    public Animator jumpAnimator;

    public float acc_thresh;
    public float footStep;

    //jumpの3段階分け用閾値
    public float jumpTime1;
    public float jumpTime2;

    public float jumpPress1;
    public float jumpPress2;

    private Vector3 acc_prev;
    private Vector3 static_acc;

    private Quaternion q_i;

   // StreamWriter sw;

    protected override void Start()
    {
        Ability.Add("motion");
        Ability.Add("push");
        base.Start();
        static_acc = new Vector3(-1, -1f, -1f);

        q_i = new Quaternion(0f, 0f, 0f, 1f);

        jumpAnimator = transform.GetChild(0).GetComponent<Animator>();
        //jumpAnimator.enabled = false;

        //sw = new StreamWriter("./AccData.csv", false);
        //sw.WriteLine("x,y,z,m");

    }

    private void Update()
    {
        if (Input.GetKeyUp(KeyCode.P))
        {
            q_i *= Quaternion.Inverse(transform.rotation);
        }

    }

    public void onClickInitButton()
    {
        q_i *= Quaternion.Inverse(transform.rotation);
    }



    public override void OnReceive(string data, string address, int port)
    {
        var type = JsonUtility.FromJson<NekoProtocol>(data);


        switch (type.type)
        {
            case 0: //motionデータがきたとき
                {
                    var motion = JsonUtility.FromJson<SensorProtocol>(data);

                    Vector3 v = new Vector3(motion.acc.x, motion.acc.y, motion.acc.z);
                    //v.Normalize();

                    //Debug.Log(v.magnitude.ToString("F4"));

                    //sw.WriteLine(v.x + "," + v.y + "," + v.z + "," + v.magnitude);

                    // 右手系 → 左手系変換して代入
                    //transform.rotation = new Quaternion(-sensor.rot.x, sensor.rot.y, sensor.rot.z, -sensor.rot.w);
                    //        transform.rotation = new Quaternion(-sensor.rot.x, sensor.rot.y, -sensor.rot.z, sensor.rot.w);
                    transform.rotation = q_i * new Quaternion(motion.rot.x, -motion.rot.z, -motion.rot.y, motion.rot.w); //fuji
                    //DuckObj.transform.localRotation = q_i * new Quaternion(motion.rot.x, -motion.rot.z, -motion.rot.y, motion.rot.w); //fuji

                    //軸合わせ試す
                    //transform.position = new Vector3(transform.position.x, transform.position.y, -transform.position.z);
                    //transform.Rotate(new Vector3(0f, -90f, 0f));
                    //transform.Rotate(new Vector3(0f, 0f, -90f));

                    if (static_acc.x == -1f && static_acc.y == -1f && static_acc.z == -1f)
                    {
                        static_acc = new Vector3(motion.acc.x, motion.acc.y, motion.acc.z);
                    }


                    if (acc_prev == Vector3.zero)
                    {
                        //acc_prev = v;
                    }
                    else
                    {

                        Vector3 acc_diff = v - acc_prev;

                        //vを正規化しないで、nolmの1G(静止状態の重力分)からの差が閾値だったら、にする
                        if (v.magnitude < acc_thresh)
                        {
                            transform.position += transform.forward * footStep;
                            transform.localPosition = new Vector3(transform.localPosition.x, 0.063f, transform.localPosition.z); //高さはplaygroundの高さに固定
                            //DuckObj.transform.localPosition += DuckObj.transform.forward * footStep;
                        }

                    }
                    acc_prev = v;
                }
                break;
            case 1: //pushデータがきたとき
                {
                    var pressure = JsonUtility.FromJson<PressureProtocol>(data);
                    var push = pressure.push;

                    Debug.Log(push.result + ", " + push.time + ", " + push.MaxDiff);

                    switch (push.result)
                    {

                        case 1:
                            break;
                        case 2:

                            if ((push.time >= 0f && push.time <= jumpTime1) || (push.MaxDiff >= 0f && push.MaxDiff <= jumpPress1))
                            {
                                Debug.Log("Low");
                                jumpAnimator.SetTrigger("lowTrigger");
                                //jumpAnimator.Play("Jump_low");
                            }
                            else if ((push.time >= jumpTime1 && push.time <= jumpTime2) || (push.MaxDiff >= jumpPress1 && push.MaxDiff <= jumpPress2))
                            {
                                Debug.Log("Mid");
                                jumpAnimator.Play("Jump_mid");
                            }
                            else if (push.time >= jumpTime2 || push.MaxDiff >= jumpPress2)
                            {
                                Debug.Log("High");
                                jumpAnimator.Play("Jump_high");
                            }
                            break;

                        default:
                            break;
                    }



                }
                break;
            default:
                break;
        }
    }

    private void OnDestroy()
    {
        //if (sw != null)
        //{
        //    sw.Flush();
        //    sw.Close();
        //}
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
    public struct PushProtocol
    {
        public int result;
        public float time;
        public float MaxDiff;
    }


    [System.Serializable]
    public class NekoProtocol
    {
        public int type;
    }

    [System.Serializable]
    public class SensorProtocol : NekoProtocol
    {
        public int time;
        [SerializeField]
        public GyroProtocol gyro;
        [SerializeField]
        public AccProtocol acc;
        [SerializeField]
        public PostureRotProtocol rot;
    }

    [System.Serializable]
    public class PressureProtocol : NekoProtocol
    {
        [SerializeField]
        public PushProtocol push;
    }
}

