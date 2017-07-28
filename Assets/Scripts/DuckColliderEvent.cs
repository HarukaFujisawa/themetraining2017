using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DuckColliderEvent : MonoBehaviour {

    //エフェクトのプレハブへのパス
    private const string EFFECT_PATH = "Effect/ParticleRibbon/Charge_03";
    GameObject effect;
    AudioSource audiosource;
    RemoteMotionSensor remoteMotionSensor;


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
        Debug.Log(collider.gameObject.name + " enter!!");

        effect = Instantiate(Resources.Load(EFFECT_PATH)) as GameObject;
        effect.transform.SetParent(this.transform.parent);
        effect.transform.localPosition = new Vector3(0f, 0.1f, 0f);
        effect.transform.localScale = new Vector3(0.05f, 0.11f, 0.05f);

        audiosource.Play();

        //ぶつかったことをセンサ側に通達
        //remoteMotionSensor.Send("Conflict!! to " + remoteMotionSensor.Target.Address);
        remoteMotionSensor.Send("{\"event\":\"conflictEnter\"}");

    }
    private void OnTriggerExit(Collider collider)
    {
        Debug.Log(collider.gameObject.name + " exit..");
        if(effect != null)
        {
            Destroy(effect);

            if(audiosource != null && audiosource.isPlaying)
            {
                audiosource.Stop();
            }
        }

        //ぶつかったことをセンサ側に通達
        remoteMotionSensor.Send("{\"event\":\"conflictExit\"}");

    }
}
