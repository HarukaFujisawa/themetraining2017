using System.Collections;
using System.Collections.Generic;

using UnityEngine;

using Slack;

public class SendMessageToSlack : MonoBehaviour {

    public string sendText = "Unityから送信...";
    public string _token = "0000";
    public string _channel = "D4WNU3BK7"; //"@haruka";


    // Use this for initialization
    void Start()
    {
        WebSocketConnectionViaSlack.instance.sendMessage(_channel, sendText);
        //sendMessage(sendText); //http
    }

    // Update is called once per frame
    void Update() {

    }

    void sendMessage(string message)
    {

        var data = new PostMessageData
        {
            token = _token,
            channel = _channel,
            text = message,
            parse = string.Empty,
            link_names = string.Empty,
            username = string.Empty,
            icon_url = string.Empty,
            icon_emoji = string.Empty
        };

        var routine = SlackAPI.PostMessage(data);
        StartCoroutine(routine);

        //var wsdata = new wsPostMessageData
        //{
        //    id = "1",
        //    type = "message",
        //    channel = "D4WNU3BK7", //@haruka  "C4WNTPLQM", //random
        //    text = message
        //};

        //WebSocketConnectionViaSlack.instance.ws.Send(JsonUtility.ToJson(wsdata));

    }
}
