using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Initialize : MonoBehaviour {

    public GameObject Duck1;
    public GameObject Duck2;

    public GameObject playground;


    public void onClickInitButton()
    {
        Duck1.transform.position = playground.transform.position + new Vector3(0.1f, 0.02f, 0f);
        Duck2.transform.position = playground.transform.position + new Vector3(-0.1f, 0.02f, 0f);

    }
}
