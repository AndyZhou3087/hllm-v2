package com.kuxx.hllm;

import java.io.IOException;

import org.apache.http.HttpResponse;
import org.apache.http.HttpVersion;
import org.apache.http.client.methods.HttpRequestBase;
import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.conn.params.ConnManagerParams;
import org.apache.http.conn.scheme.PlainSocketFactory;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.params.HttpProtocolParams;
import org.apache.http.protocol.HTTP;

import android.content.Context;
import android.util.Log;

public class HttpRequestBox {
	private static final String TAG = "HttpRequestBox";

	private static final int DEFAULT_RETRY_TIMES = 3;// retry three times
	private static final long CONN_MGR_TIMEOUT = 5 * 1000L;// 5s
	private static final int CONN_TIMEOUT = 5 * 1000;// 5s
	private static final int SO_TIMEOUT = 5 * 1000;// 5s
	private long mLastReconnectMS = 0;
	private static final long MIN_RECONNECT_MS = 5 * 1000L;// 5s

	private static HttpRequestBox mInstance = null;
	private static final byte[] mLock = new byte[0];

	private DefaultHttpClient mHttpClient = null;

	private HttpRequestBox(Context ctx) {
		SchemeRegistry schemeRegistry = new SchemeRegistry();
		schemeRegistry.register(new Scheme("http", PlainSocketFactory
				.getSocketFactory(), 80));
		HttpParams params = new BasicHttpParams();
		HttpProtocolParams.setVersion(params, HttpVersion.HTTP_1_1);
		HttpProtocolParams.setContentCharset(params, HTTP.UTF_8);
		HttpProtocolParams.setUseExpectContinue(params, false);
		HttpConnectionParams.setConnectionTimeout(params, CONN_TIMEOUT);
		HttpConnectionParams.setSoTimeout(params, SO_TIMEOUT);
		ConnManagerParams.setTimeout(params, CONN_MGR_TIMEOUT);
		ClientConnectionManager ccm = new ThreadSafeClientConnManager(params,
				schemeRegistry);
		mHttpClient = new DefaultHttpClient(ccm, params);
	}

	public static HttpRequestBox getInstance(Context ctx) {
		synchronized (mLock) {
			if (mInstance == null) {
				mInstance = new HttpRequestBox(ctx);
			}
		}
		return mInstance;
	}

	public HttpResponse sendRequest(HttpRequestBase request) {
		HttpResponse response = null;
		for (int i = 0; i < DEFAULT_RETRY_TIMES; i++) {
			try {
				Log.d(TAG, "request url : "
						+ request.getURI().toURL().toString());
				response = mHttpClient.execute(request);
				break;
			} catch (java.net.SocketException e) {
				long now = System.currentTimeMillis();
				if (now - mLastReconnectMS > MIN_RECONNECT_MS) {
					mLastReconnectMS = now;
				} else {
					break;
				}
			} catch (IOException e) {
				Log.e(TAG, "IOException", e);
			}
			// delay one second for another request
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				Log.e(TAG, "InterruptedException ", e);
				break;
			}
		}// ?end for
		return response;
	}
}
