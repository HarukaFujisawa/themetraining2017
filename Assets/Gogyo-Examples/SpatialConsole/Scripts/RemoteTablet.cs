using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Gogyo.Network;
using System;

public class RemoteTablet : PeripheralDevice {

    //public string[] ability;

    public GameObject me; //自分の人形
    public int divWidth = 5; 
    public int divHeight = 3;
    public float posY = 0.4f; //yは常に0.063f
    public float rad = 0.04f;//タッチした場所とネコ表示する位置のオフセットを得るための、ネコ台座の半径

    float parentScale;
    //playgroundのサイズ
    float pgWidth;
    float pgHeight;
    //ボタン1つのサイズ
    float areaWidth;
    float areaHeight;

    Vector3 currentPos;
    Vector3 targetPos; //移動先の場所

    public override void OnReceive(string data, string address, int port)
    {
        Debug.Log(data + " from " + address + " : " + port);

        var position = JsonUtility.FromJson<IndexPositionProtocol>(data);

        //今向いてる方向とは逆向きに、ネコ土台の半径の長さ分、オフセット
        me.transform.localPosition = getPosition(position) - me.transform.forward * rad / parentScale;

    }

    // Use this for initialization
    protected override void Start () {

        //for (int i = 0; i < ability.Length; i++)
        //{
        //    Ability.Add(ability[i]);
        //}
        //Ability.Add("tablet");

        pgWidth = transform.parent.GetComponent<Vuforia.ImageTargetBehaviour>().GetSize().x;
        pgHeight = transform.parent.GetComponent<Vuforia.ImageTargetBehaviour>().GetSize().y;
        //pgWidth = GetComponent<Vuforia.ImageTargetBehaviour>().GetSize().x;
        //pgHeight = GetComponent<Vuforia.ImageTargetBehaviour>().GetSize().y;
        areaWidth = pgWidth / divWidth;
        areaHeight = pgHeight / divHeight;
        //parentScale = transform.localScale.x;
        parentScale = transform.parent.localScale.x;
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
        //Vector3 pos = new Vector3((pgWidth * id.idX - pgWidth / 2) / parentScale, posY, (pgHeight * id.idY - pgHeight / 2) / parentScale);

        return pos;
    }
}

[System.Serializable]
public class IndexPositionProtocol
{
    public int idX;
    public int idY;
}

