﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Initialize : MonoBehaviour {

    static public Initialize instance;


    public GameObject Duck1;
    public GameObject Duck2;

    public GameObject playground;


    void Awake()
    {
        if (instance == null)
        {

            instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {

            Destroy(gameObject);
        }

    }


    private void Update()
    {
        if (Input.GetKeyUp(KeyCode.I))
        {
            onClickInitButton();
        }

        if(Input.GetKeyUp(KeyCode.O))
        {
            Vuforia.VuforiaBehaviour.Instance.enabled = !Vuforia.VuforiaBehaviour.Instance.enabled;
        }
    }


    public void onClickInitButton()
    {
        Duck1.transform.position = playground.transform.position + new Vector3(0.1f, 0.02f, 0f);
        Duck2.transform.position = playground.transform.position + new Vector3(-0.1f, 0.02f, 0f);
        Duck1.GetComponent<RemoteMotionSensor>().SetQi();
        Duck2.GetComponent<RemoteMotionSensor>().SetQi();
    }

    public void OnApplicationQuit()
    {
        Vuforia.VuforiaBehaviour.Instance.enabled = true;
    }
}
