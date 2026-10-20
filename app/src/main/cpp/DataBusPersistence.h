#ifndef DATABUSPERSISTENCE_H
#define DATABUSPERSISTENCE_H

#include <string>
#include <vector>
#include <map>


class DataBusPersistence {
public:
    static bool save();
    static bool load();
    static bool exists();
    static bool deleteFile();
    static long getLastModified();
    static long getFileSize();

private:
    DataBusPersistence() = delete; // 工具类禁止实例化

    static const std::string SAVE_PATH;

    // 内部二进制流封装
    class BinaryWriter;
    class BinaryReader;

    static void saveToFile(const std::string& path);
    static void loadFromFile(const std::string& path);

    static void saveState(BinaryWriter& out);
    static void loadState(BinaryReader& in);
    static void writeListOfIntLists(BinaryWriter& out, const std::vector<std::vector<int>>& list);
    static std::vector<std::vector<int>> readListOfIntLists(BinaryReader& in);
    static void writeStringList(BinaryWriter& out, const std::vector<std::string>& list);
    static std::vector<std::string> readStringList(BinaryReader& in);
    static void writeString(BinaryWriter& out, const std::string& s);
    static std::string readString(BinaryReader& in);
};

#endif