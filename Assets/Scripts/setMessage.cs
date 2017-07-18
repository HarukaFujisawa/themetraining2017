using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using Slack;

public class setMessage : MonoBehaviour {

    public Text text;
    public string userID = "U4WJ6147M"; //haruka: U4WJ6147M gota: U55CYJMV0 yuya: U4WL56UTC
    private string currentTS;

    // Use this for initialization
    void Start () {
		
	}

    // Update is called once per frame
    void Update() {

        if (WebSocketConnectionViaSlack.instance.latestMessageDict.ContainsKey(userID))
        {
            wsGetMessageData data = WebSocketConnectionViaSlack.instance.latestMessageDict[userID];

            if(currentTS == null)
            {
                currentTS = data.ts;
            }
            else if (!currentTS.Equals(data.ts))
            {
                text.text = data.text;
            }
        }
    }
}
