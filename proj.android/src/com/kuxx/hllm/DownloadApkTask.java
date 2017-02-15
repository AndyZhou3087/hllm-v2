package com.kuxx.hllm;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import org.apache.http.Header;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.methods.HttpGet;


import android.content.Context;
import android.os.AsyncTask;

public class DownloadApkTask extends AsyncTask<String, Long, String> {
	//private static final String TAG = "DownloadApkTask";

	public interface onDownloadApkListener {
		public void onUpdateProgress(Long... values);

		public void onDownloadFinished(String destFilePath);
	}

	private static final String FILE_TEMP_SUFFIX = ".temp";
	private static final String HTTP_HEADER_CONTENT_LENGTH = "Content-Length";

	private Context mCtx;
	private String url;
	private String mDestFilePath;
	private onDownloadApkListener listener;

	// file size
	private long mFileSize;
	private long mDownloadedSize;
	
	//HttpURLConnection conn;

	public DownloadApkTask(onDownloadApkListener listener) {
		this.mCtx = (Context) listener;
		this.listener = listener;
		mFileSize = 0;
		mDownloadedSize = 0;
	}

	@Override
	protected String doInBackground(String... params) {
		this.url = params[0];
		this.mDestFilePath = params[1];
		HttpGet get = null;
		HttpResponse resp = null;
		File destFile = null;
		InputStream is = null;
		try {
			destFile = new File(mDestFilePath + FILE_TEMP_SUFFIX);
			if (destFile.exists()) {
				// if destFile exist ,delete it first
				destFile.delete();
			}
			destFile.createNewFile();
			get = new HttpGet(url);
			resp = HttpRequestBox.getInstance(mCtx).sendRequest(get);
			if (resp == null) 
			{
				return null;
			}
			int statusCode = resp.getStatusLine().getStatusCode();
			if (statusCode < HttpStatus.SC_OK
					&& statusCode >= HttpStatus.SC_MULTIPLE_CHOICES) {
				return null;
			}
			mFileSize = parseFileSize(resp);
			is = resp.getEntity().getContent();
			/*URL urlObj = new URL(url);
			conn = (HttpURLConnection) urlObj.openConnection();

			conn.setConnectTimeout(10000);
			conn.setRequestMethod("GET");*/
			//conn.setRequestProperty("Accept", "*/*");
			/*conn.setRequestProperty("Accept-Language", "zh-CN");
			conn.setRequestProperty("Charset", "UTF-8");
			conn.setRequestProperty("User-Agent", System.getProperties()
					.getProperty("http.agent"));
			conn.setRequestProperty("Connection", "Keep-Alive");
			conn.connect();
			mFileSize = conn.getContentLength();
			MyLog.d("", "mFileSize : " + mFileSize + ",code="+conn.getResponseCode());
			if (conn.getResponseCode() == HttpURLConnection.HTTP_OK) {
				is = conn.getInputStream();
			}*/

			return saveApkFile(destFile, is);
		} catch (Exception e) {
			if (destFile != null) {
				destFile.delete();
			}
			return null;
		} finally {
			if (is != null) {
				try {
					is.close();
					//conn.disconnect();
				} catch (IOException e) {
					//
				}
			}
		}

	}

	@Override
	protected void onProgressUpdate(Long... values) {
		super.onProgressUpdate(values);
		if (listener != null) {
			listener.onUpdateProgress(values);
		}
	}

	@Override
	protected void onPostExecute(String result) {
		super.onPostExecute(result);
		if (listener != null) {
			listener.onDownloadFinished(result);
		}
	}

	private int parseFileSize(HttpResponse resp) {
//		Header[]  header1 =resp.getAllHeaders();
//		for (int i=0;i<header1.length; i++)
//		{
//			Log.d("", header1[i].getName() + ":"+header1[i].getValue());
//		}
		Header header = resp.getFirstHeader(HTTP_HEADER_CONTENT_LENGTH);
		if (header != null) {
			return Integer.valueOf(header.getValue());
		}
		return -1;
	}

	private String saveApkFile(File destFile, InputStream is)
			throws FileNotFoundException, IOException {
		FileOutputStream fos = null;
		byte[] buf = new byte[4096];
		int len = 0;
		fos = new FileOutputStream(destFile);
		long lastReceive = System.currentTimeMillis();
		while ((len = is.read(buf)) != -1) {
			fos.write(buf, 0, len);
			mDownloadedSize += len;
			long now = System.currentTimeMillis();
			if (now - lastReceive > 500 || mDownloadedSize == mFileSize) {
				lastReceive = now;
				publishProgress(mFileSize, mDownloadedSize);
			}
		}
		fos.close();
		// rename
		String destFilePath = destFile.getAbsolutePath();
		int index = destFilePath.indexOf(FILE_TEMP_SUFFIX);
		String newFileName = destFilePath.substring(0, index);
		destFile.renameTo(new File(newFileName));
		return newFileName;
	}
}
