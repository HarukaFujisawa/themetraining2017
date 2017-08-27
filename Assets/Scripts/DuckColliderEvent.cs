using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DuckColliderEvent : MonoBehaviour {

    //エフェクトのプレハブへのパス
    private const string EFFECT_PATH = "Effect/ParticleRibbon/Charge_03";
    GameObject effect;
    AudioSource audiosource;
    RemoteMotionSensor remoteMotionSensor;

    public Color LEDColor_enter;
    public Color LEDColor_exit;


    // Use this for initialization
    void Start () {
        audiosource = GetComponent<AudioSource>();
        remoteMotionSensor = GetComponentInParent<RemoteMotionSensor>();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    private void OnTriggerEnter(Collider collider)
    {
        if (remoteMotionSensor != null)
        {

            Debug.Log(collider.gameObject.name + " enter!!");

            effect = Instantiate(Resources.Load(EFFECT_PATH)) as GameObject;
            effect.transform.SetParent(this.transform.parent);
            effect.transform.localPosition = new Vector3(0f, -0.03f, 0f);
            effect.transform.localScale = new Vector3(0.05f, 0.11f, 0.05f);

            audiosource.Play();

            GetComponent<Renderer>().material.color = LEDColor_enter; //ピンク

            //ぶつかったことをセンサ側に通達 1回だと受け取れないことがあるから2回送る
            //remoteMotionSensor.Send("Conflict!! to " + remoteMotionSensor.Target.Address);
            //remoteMotionSensor.Send("{\"event\":\"conflictEnter\"}");
            string sendMessage = "{\"event\":{\"onLED\":[" + (LEDColor_enter.r * 255).ToString() + ","
                                                          + (LEDColor_enter.g * 255).ToString() + ","
                                                          + (LEDColor_enter.b * 255).ToString() + "]}}";
            remoteMotionSensor.Send(sendMessage);
            remoteMotionSensor.Send(sendMessage);
            Debug.Log(sendMessage + " " + gameObject.transform.parent.name);
        }

    }
    private void OnTriggerExit(Collider collider)
    {
        if (remoteMotionSensor != null)
        {

            Debug.Log(collider.gameObject.name + " exit..");
            if (effect != null)
            {
                Destroy(effect);

                if (audiosource != null && audiosource.isPlaying)
                {
                    audiosource.Stop();
                }
            }

            GetComponent<Renderer>().material.color = LEDColor_exit;


            //ぶつかったことをセンサ側に通達 1回だと受け取れないことがあるから2回送る
            //remoteMotionSensor.Send("{\"event\":\"conflictExit\"}");
            string sendMessage = "{\"event\":{\"onLED\":[" + (LEDColor_exit.r * 255).ToString() + ","
                                                          + (LEDColor_exit.g * 255).ToString() + ","
                                                          + (LEDColor_exit.b * 255).ToString() + "]}}";
            remoteMotionSensor.Send(sendMessage);
            remoteMotionSensor.Send(sendMessage);
            Debug.Log(sendMessage + " " + gameObject.transform.parent.name);
        }

    }

    private void OnApplicationQuit()
    {
        if (remoteMotionSensor != null)
        {
            remoteMotionSensor.Send("{\"event\":\"offLED\"}");
            remoteMotionSensor.Send("{\"event\":\"offLED\"}");
        }
    }
}
