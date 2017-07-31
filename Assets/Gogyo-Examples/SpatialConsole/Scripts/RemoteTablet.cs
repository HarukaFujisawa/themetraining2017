using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Gogyo.Network;
using System;

public class RemoteTablet : PeripheralDevice {

    public GameObject other; //相手の人形
    public int divWidth; //3
    public int divHeight;//2
    public float posY = 0.063f; //yは常に0.063f

    float parentScale;
    //playgroundのサイズ
    float pgWidth;
    float pgHeight;
    //ボタン1つのサイズ
    float areaWidth;
    float areaHeight;

    public override void OnReceive(string data, string address, int port)
    {
        Debug.Log(data + " from " + address + " : " + port);

        var position = JsonUtility.FromJson<IndexPositionProtocol>(data);
        //相手の位置をidの位置にする 
        other.transform.localPosition = getPosition(position);

    }

    // Use this for initialization
    protected override void Start () {
        Ability.Add("tablet");
        pgWidth = GetComponent<Vuforia.ImageTargetBehaviour>().GetSize().x;
        pgHeight = GetComponent<Vuforia.ImageTargetBehaviour>().GetSize().y;
        areaWidth = pgWidth / divWidth;
        areaHeight = pgHeight / divHeight;
        parentScale = transform.localScale.x;
        base.Start();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    // 2 1 0
    // 5 4 3
    Vector3 getPosition(IndexPositionProtocol id)
    {
        Vector3 pos = new Vector3((pgWidth / 2 - areaWidth * id.idX - areaWidth / 2) / parentScale, posY, (pgHeight / 2 - areaHeight * id.idY - areaHeight / 2) / parentScale);

        return pos;
    }
}

[System.Serializable]
public class IndexPositionProtocol
{
    public int idX;
    public int idY;
}

