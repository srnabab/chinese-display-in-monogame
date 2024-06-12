using System;
using System.Runtime.InteropServices;
using System.Text;

namespace a;
public class F
{
     [DllImport("hash.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void create_hash();

    [DllImport("hash.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void read_hash_from_json([MarshalAs(UnmanagedType.LPStr)] string filename);

    private const string dllPath_Font = "C:/D/demo/demo_2/demo_2/bin/Debug/net6.0-windows/hash.dll";
    [DllImport(dllPath_Font, EntryPoint = "FFF", CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr FFF(IntPtr ch);

    [DllImport(dllPath_Font, EntryPoint = "font_free", CallingConvention = CallingConvention.Cdecl)]
    private static extern void font_free(IntPtr ptr);

    [DllImport(dllPath_Font, EntryPoint = "strlen_use", CallingConvention = CallingConvention.Cdecl)]
    private static extern int strlen_use(IntPtr ptr);

    [DllImport(dllPath_Font, EntryPoint = "read_hash_from_json", CallingConvention = CallingConvention.Cdecl)]
    private static extern void read_hash_from_json(IntPtr ptr, IntPtr filename);



    private static IntPtr Result_p;
    private static int size;

    public static int [] Font_Num(string Input)
    {
        // 调用 C 函数
        create_hash();

        // 从 JSON 文件读取并插入数据到哈希表
        string jsonFilePath = "chinese_hash.json";
        read_hash_from_json(jsonFilePath);

        // 将字符串转换为UTF-8字节数组
        byte[] utf8Bytes = Encoding.UTF8.GetBytes(Input);

        // 分配非托管内存，并复制字节数组
        IntPtr utf8Ptr = Marshal.AllocHGlobal(utf8Bytes.Length + 1);
        Marshal.Copy(utf8Bytes, 0, utf8Ptr, utf8Bytes.Length);
        Marshal.WriteByte(utf8Ptr, utf8Bytes.Length, 0);  // Null-terminate the string

        // 调用内部C函数
        size = strlen_use(utf8Ptr);
        Result_p = FFF(utf8Ptr);

        int [] result = new int[size];
        Marshal.Copy(Result_p, result, 0, size);

        // 释放非托管理内存
        Marshal.FreeHGlobal(utf8Ptr);

        return result;
    }

    public static void Free()
    {
        font_free(Result_p);
    }   

    public static int Strlen()
    {
        return size;
    }
}