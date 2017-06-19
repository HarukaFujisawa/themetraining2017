using System;
using System.Collections;
using UnityEngine;

namespace Slack
{
    [Serializable]
    public struct PostMessageData
    {
        public string token;// = string.Empty;
        public string channel;// = string.Empty;
        public string text;// = string.Empty;
        public string parse;// = string.Empty;
        public string link_names;// = string.Empty;
        public string username;// = string.Empty;
        public string icon_url;// = string.Empty;
        public string icon_emoji;// = string.Empty;
    }

    public struct GetMessageData
    {
        public string token;// = string.Empty;
        public string channel;// = string.Empty;
        public string latest;// = string.Empty;
        public string oldest;// = string.Empty;
        public string inclusive;// = string.Empty;
        public string count;// = string.Empty;
        public string unreads;// = string.Empty;
    }

    public struct wsPostMessageData
    {
        public string id;
        public string type;
        public string channel;
        public string text;
    }

    [System.Serializable]
    public class wsGetMessageData
    {
        public string type;
        public string channel;
        public string user;
        public string text;
        public string ts;
        public string source_team;
        public string team;
    }

    [Serializable]
    public struct rtmConnect
    {
        public string ok;
        public string url;
        public Team team;
        public Self self;
    }


    [Serializable]
    public struct Team
    {
        public string id;
        public string name;
        public string domain;
    }

    [Serializable]
    public struct Self
    {
        public string id;
        public string name;
    }
       


    public static class SlackAPI
    {
        public static IEnumerator PostMessage(
            PostMessageData data,
            Action onSuccess = null,
            Action<string> onError = null
        )
        {
            var form = new WWWForm();
            form.AddField("token", data.token);
            form.AddField("channel", data.channel);
            form.AddField("text", data.text);
            form.AddField("parse", data.parse);
            form.AddField("link_names", data.link_names);
            form.AddField("username", data.username);
            form.AddField("icon_url", data.icon_url);
            form.AddField("icon_emoji", data.icon_emoji);

            var url = "https://slack.com/api/chat.postMessage"; //※社内ネットワークからだとproxyが邪魔してアクセスできない
            var www = new WWW(url, form);
            yield return www;
            var error = www.error;

            Debug.Log(System.Text.Encoding.ASCII.GetString(www.bytes)); //->json取ってくる

            if (!string.IsNullOrEmpty("postMessage: " + error))
            {
                Debug.Log(error);

                if (onError != null)
                {
                    onError(error);
                }
                yield break;
            }

            if (onSuccess != null)
            {
                onSuccess();
            }
        }

        public static IEnumerator getMessage(
            GetMessageData data,
            Action<string> onSuccess = null,
            Action<string> onError = null
        )
        {
            var form = new WWWForm();
            form.AddField("token", data.token);
            form.AddField("channel", data.channel);
            form.AddField("latest", data.latest);
            form.AddField("oldest", data.oldest);
            form.AddField("inclusive", data.inclusive);
            form.AddField("count", data.count);
            form.AddField("unreads", data.unreads);

            var url = "https://slack.com/api/channels.history"; //※社内ネットワークからだとproxyが邪魔してアクセスできない
            var www = new WWW(url, form);
            yield return www;
            var error = www.error;

            string result = System.Text.Encoding.ASCII.GetString(www.bytes); //->json取ってくる

            if (!string.IsNullOrEmpty(error))
            {
                //Debug.Log("getMessage: " + error);

                if (onError != null)
                {
                    onError(error);
                }
                yield break;
            }

            if (onSuccess != null)
            {
                onSuccess(result);
            }
        }

        public static IEnumerator getWebSocketURL(
            string token,
            Action<rtmConnect> onSuccess = null,
            Action<string> onError = null
        )
        {
            var form = new WWWForm();
            form.AddField("token", token);

            var url = "https://slack.com/api/rtm.connect"; //※社内ネットワークからだとproxyが邪魔してアクセスできない
            var www = new WWW(url, form);
            yield return www;
            var error = www.error;

            if (!string.IsNullOrEmpty(error))
            {

                if (onError != null)
                {
                    onError(error);
                }
                yield break;
            }

            string result = System.Text.Encoding.ASCII.GetString(www.bytes); //->json取ってくる
            //jsonをparse
            rtmConnect connect = JsonUtility.FromJson<rtmConnect>(result);
            yield return connect;

            if (onSuccess != null)
            {
                onSuccess(connect);
            }
        }
    }
}