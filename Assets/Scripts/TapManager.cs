using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using HoloToolkit.Unity.InputModule;

public class TapManager : MonoBehaviour, IInputClickHandler {

	// Use this for initialization
	void Start () {
		InputManager.Instance.PushFallbackInputHandler (gameObject);
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void OnInputClicked(InputClickedEventData eventData)
	{
		//アヒルちゃんを生成
		// プレハブを取得
		GameObject prefab = (GameObject)Resources.Load ("Prefabs/Rubber_Duck");
        prefab.transform.position = GazeManager.Instance.HitPosition + new Vector3(0f, 0.1f, 0f);
        //prefab.transform.LookAt(Camera.main.transform);
        //prefab.transform.Rotate(new Vector3(-90, 90, 0));
        // プレハブからインスタンスを生成
        Instantiate(prefab);
	}

}
