package com.example.nativwapp.util;

import android.util.Log;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Collection;
import java.util.Iterator;
import java.util.TreeMap;

/**
 * MD5加密工具类
 */
public class EncryptUtils {

    public static void main(String[] args) {
        TreeMap<String, String> params = new TreeMap<>();
        params.put("api_version", "1.0");
        params.put("api_id", "999999");
        params.put("oid", "352575071991660");
        params.put("source", "android");
        params.put("timestamp", "1477892707861");
        params.put("verifymethod", "md5");
        params.put("version", "v1.3.6");
        String sign = EncryptUtils.signByMd5("123456", params);
        System.out.println("sign=" + sign);
        params.put("sign", EncryptUtils.signByMd5("123456", params));
        System.out.println(params);
    }


    /**
     * MD5签名
     *
     * @param secret  秘钥
     * @param paraMap 参数
     * @return String
     */
    public static String signByMd5(String secret, TreeMap<String, String> paraMap) {
        try {
            String signContent = getContent(secret, paraMap);
            Log.d("sign = ", signContent);
            return hash(signContent);
        } catch (UnsupportedEncodingException | NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * MD5兩次签名
     *
     * @param secret  秘钥
     * @param paraMap 参数
     * @return String
     */
    public static String signByMd5Twice(String secret, TreeMap<String, String> paraMap) {
        try {
            String signContent = getContent(secret, paraMap);
//            LogUtils.d("sign = ", signContent);
            return hash(hash(signContent));
        } catch (UnsupportedEncodingException | NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * MD5
     *
     * @param signContent 验签内容
     * @return String
     * @throws NoSuchAlgorithmException
     * @throws UnsupportedEncodingException
     */
    private static String hash(String signContent) throws NoSuchAlgorithmException, UnsupportedEncodingException {
        MessageDigest md = MessageDigest.getInstance("MD5");
        md.update(signContent.getBytes("UTF-8"));
        byte[] bytes = md.digest();
        StringBuilder sb = new StringBuilder();
        for (byte data : bytes) {
            sb.append(Integer.toString((data & 0xff) + 0x100, 16).substring(1));
        }
        return sb.toString().toLowerCase();
    }


    /**
     * 获取参数串
     *
     * @param secret  秘钥
     * @param treeMap 参数
     * @return String
     */
    private static String getContent(String secret, TreeMap<String, String> treeMap) {
        Collection keys = treeMap.keySet();
        Iterator it = keys.iterator();

        String strsign = "";
        while (it.hasNext()) {
            String key = (String) it.next();
            String value = treeMap.get(key);
            if (value == null) {
                value = "";
                treeMap.put(key, value);
            }
            strsign = strsign + "&" + key + "=" + value;
        }
        return strsign.substring(1) + secret;
    }

    /**
     * 获取参数串
     *
     * @param secret  秘钥
     * @param treeMap 参数
     * @return String
     */
    private static String getContentByValueEnCode(String secret, TreeMap<String, String> treeMap) {
        Collection keys = treeMap.keySet();
        Iterator it = keys.iterator();

        String strsign = "";
        while (it.hasNext()) {
            String key = (String) it.next();
            String value = treeMap.get(key);
            if (value == null) {
                value = "";
                treeMap.put(key, value);
            }
            strsign = strsign + "&" + key + "=" + URLEncoder.encode(value);
        }
        return strsign.substring(1) + secret;
    }
}
