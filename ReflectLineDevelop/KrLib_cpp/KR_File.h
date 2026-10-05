/*
   - KR_File.h - (C++)
   ver.2026/09/03

   ファイルの読み取り, 書き出し機能。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_CPP_GLOBAL
  #include "KR_Global.h"
#endif

//[include] ".h"ファイルで使うもの.
#include <fstream>
#include <filesystem>

//KrLib名前空間.
namespace KR
{
    /*
       ファイルを開くモード.

       ↓フラグ方式で記述する.
       file.Open(path, FileOpenMode::Out | FileOpenMode::App);
    */
    enum class FileOpenMode
    {
        None = 0,

        //[基本モード]
        In     = 1 << 0, //読み込み.
        Out    = 1 << 1, //書き込み.
        App    = 1 << 2, //ファイル末尾へ追記.
        Trunc  = 1 << 3, //開いた時にファイル内容を空にする.
        Binary = 1 << 4, //バイナリモード.

        //[汎用]
        Read  = In,          //読み込み.
        Write = Out | Trunc, //書き込み(既存内容を消去)
    };
    //OR演算子で記述できるように.
    inline FileOpenMode operator|(FileOpenMode lhs, FileOpenMode rhs)
    {
        using T = std::underlying_type_t<FileOpenMode>;

        return static_cast<FileOpenMode>(
            static_cast<T>(lhs) | static_cast<T>(rhs)
        );
    }

    //FileOpenModeが持つフラグを判定する.
    //enum classはビット演算できないため、一度整数型に変換して判定する.
    static bool HasFlag(FileOpenMode value, FileOpenMode flag)
    {
        //enum classの基底型(intなど)を取得.
        using T = std::underlying_type_t<FileOpenMode>;
        //指定したフラグがONならtrue.
        return (static_cast<T>(value) & static_cast<T>(flag)) != 0;
    }

    //ファイルクラス.
    class File final
    {
    //▼ ===== 変数 ===== ▼.
    private:
        MY_FSTREAM fs; //ファイルストリーム.

    //▼ ===== 関数 ===== ▼.
    public:
        //デストラクタ.
        ~File() {
            Close(); //ファイルを閉じる.
        }

        //EOFかどうか.
        bool IsEOF();

        //ファイルを開く.
        bool Open(MY_STRING filePath, FileOpenMode mode);
        //ファイルを閉じる.
        void Close();
        //ディレクトリ作成.
        void MakeDir(const std::filesystem::path& filePath);

        //読み込み, 書き込み.
        MY_STRING ReadString ();
        int       ReadInt    ();
        void      WriteString(MY_STRING data);
        void      WriteInt   (int data);
    };
}