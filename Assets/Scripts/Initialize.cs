using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Initialize : MonoBehaviour {

    static public Initialize instance;


    public GameObject Duck1;
    public GameObject Duck2;

    public GameObject playground;
    public GameObject InfoCanvas;
    MeshRenderer playgroundMesh;


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

    void Start()
    {
        playgroundMesh = playground.GetComponent<MeshRenderer>();
    }


    private void Update()
    {
        if (Input.GetKeyUp(KeyCode.I))
        {
            onClickInitButton();
        }

        if(Input.GetKeyUp(KeyCode.O))
        {
            InfoCanvas.SetActive(!InfoCanvas.activeSelf);
            playgroundMesh.enabled = !playgroundMesh.enabled;
            Vuforia.VuforiaBehaviour.Instance.enabled = !Vuforia.VuforiaBehaviour.Instance.enabled;
        }
    }


    public void onClickInitButton()
    {
        //Duck1.transform.position = playground.transform.position;// + new Vector3(0.1f, 0.02f, 0f);
        //Duck1.transform.localPosition = new Vector3(0.2f, 0.4f, 0f);
        //Duck2.transform.position = playground.transform.position;// + new Vector3(-0.1f, 0.02f, 0f);
        //Duck2.transform.localPosition = new Vector3(-0.2f, 0.4f, 0f);
        Duck1.GetComponent<RemoteMotionSensor>().SetQi();
        Duck2.GetComponent<RemoteMotionSensor>().SetQi();
    }

    public void OnApplicationQuit()
    {
        Vuforia.VuforiaBehaviour.Instance.enabled = true;
    }
}
