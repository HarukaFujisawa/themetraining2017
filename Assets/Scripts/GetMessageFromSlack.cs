using System.Collections;
using System.Collections.Generic;

using UnityEngine;

using Slack;

public class GetMessageFromSlack : MonoBehaviour
{

    public string _token = "xoxp-166685232898-166618038259-191709012435-7cd7c51730849bce2a439164c35ca1e8";
    public string _channel = "C4WNTPLQM"; //random
    public string _latest = string.Empty;
    public string _oldest = string.Empty;
    public string _inclusive = string.Empty;
    public string _count = "1";
    public string _unreads = string.Empty;


    // Use this for initialization
    void Start()
    {
        getMessage();
    }

    // Update is called once per frame
    void Update()
    {

    }

    void getMessage()
    {

        var data = new GetMessageData
        {
            token = _token,
            channel = _channel,
            latest = _latest,
            oldest = _oldest,
            inclusive = _inclusive,
            count = _count,
            unreads = _unreads
        };

        var routine = SlackAPI.getMessage(data, showGetMessage, showErrorMessage);
        StartCoroutine(routine);
    }

    void showGetMessage(string result)
    {
        Debug.Log(result);
    }

    void showErrorMessage(string error)
    {
        Debug.Log("showErrorMessage: " + error);
    }
}
