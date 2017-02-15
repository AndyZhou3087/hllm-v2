package com.kuxx.hllm;

/**
 * Http处理
 * @author
 */
public interface HttpHandle
{
    boolean HttpEventHandle(String url,int code,byte[] body);
}