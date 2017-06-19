using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectYThresh : MonoBehaviour {

    public float y_thresh = -3.0f;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {

        if(transform.position.y <= y_thresh)
        {
            Destroy(gameObject);
        }
		
	}
}
