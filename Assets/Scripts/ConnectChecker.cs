using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ConnectChecker : MonoBehaviour {

    public RemoteMotionSensor puppet1;
    public RemoteMotionSensor puppet2;
    public RemoteTablet tablet1;
    public RemoteTablet tablet2;

    public Text t_puppet1;
    public Text t_puppet2;
    public Text t_tablet1;
    public Text t_tablet2;

    public Color disconnectColor;
    public Color connectColor;


    // Use this for initialization
    void Start () {

        t_puppet1.color = disconnectColor;
        t_puppet2.color = disconnectColor;
        t_tablet1.color = disconnectColor;
        t_tablet2.color = disconnectColor;

    }

    // Update is called once per frame
    void Update () {

        if (puppet1.Target.Count > 0)
        {
            t_puppet1.color = connectColor;
        }
        else
        {
            t_puppet1.color = disconnectColor;
        }
        if (puppet2.Target.Count > 0l)
        {
            t_puppet2.color = connectColor;
        }
        else
        {
            t_puppet2.color = disconnectColor;
        }

        if (tablet1.Target.Count > 0)
        {
            t_tablet1.color = connectColor;
        }
        else
        {
            t_tablet1.color = disconnectColor;
        }
        if (tablet2.Target.Count > 0)
        {
            t_tablet2.color = connectColor;
        }
        else
        {
            t_tablet2.color = disconnectColor;
        }


    }
}
