#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>

struct File {
    int id;
    std::string name;
    std::vector<std::string> categories;
    int numCategories;
    int parent;
    int size;
};

/**
 * Task 1
 */
std::vector<std::string> leafFiles(std::vector<File> files) {
    std::vector<std::string> toReturn;
    std::map<int, bool> child;

    for (int i = 0; i < files.size(); i++) {
        child[files[i].parent] = true;
    }

    for (int i = 0; i < files.size(); i++) {
        if (!child[files[i].id]) {
            toReturn.push_back(files[i].name);
        }
    }

    return toReturn;
}

/**
 * Task 2
 */
std::vector<std::string> kLargestCategories(std::vector<File> files, int k) {
    std::vector<std::string> toReturn;

    std::map<std::string, int> categoryCount;

    for (int i = 0; i < files.size(); i++) {
        for (std::string category : files[i].categories) {
            categoryCount[category]++;
        }
    }

    std::vector<std::pair<int, std::string>> categoryPairs;

    for (auto [category, count] : categoryCount) {
        categoryPairs.push_back({count, category});
    }

    sort(categoryPairs.begin(), categoryPairs.end(),
         [](std::pair<int, std::string> a, std::pair<int, std::string> b) {
        if (a.first == b.first) {
            return a.second < b.second;
        }

        return a.first > b.first;
    });

    for (int i = 0; i < std::min((int) categoryPairs.size(), k); i++) {
        toReturn.push_back(categoryPairs[i].second);
    }

    return toReturn;
}

/**
 * Task 3
 */
int largestFileSize(std::vector<File> files) {

    std::vector<int> roots;
    std::map<int, std::vector<int>> adj;
    std::map<int, int> idToSize;

    std::function<int(int)> dfs = [&](int id) {
        int sum = idToSize[id];

        if (adj[id].size() == 0) {
            return sum;
        }

        for (int child : adj[id]) {
            sum += dfs(child);
        }

        return sum;
    };

    for (int i = 0; i < files.size(); i++) {
        if (files[i].parent == -1) roots.push_back(files[i].id);
        adj[files[i].parent].push_back(files[i].id);
        idToSize[files[i].id] = files[i].size;
    }

    int maxSize = 0;

    for (int i = 0; i < roots.size(); i++) {
        int currentSum = dfs(roots[i]);

        maxSize = std::max(maxSize, currentSum);
    }

    return maxSize;
}

int main(void) {
    std::vector<File> testFiles{
        { .id = 1, .name = "Document.txt", .categories = {"Documents"}, .numCategories = 1, .parent = 3, .size = 1024 },
        { .id = 2, .name = "Image.jpg", .categories = {"Media", "Photos"}, .numCategories = 2, .parent = 34, .size = 2048 },
        { .id = 3, .name = "Folder", .categories = {"Folder"}, .numCategories = 1, .parent = -1, .size = 0 },
        { .id = 5, .name = "Spreadsheet.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 4096 },
        { .id = 8, .name = "Backup.zip", .categories = {"Backup"}, .numCategories = 1, .parent = 233, .size = 8192 },
        { .id = 13, .name = "Presentation.pptx", .categories = {"Documents", "Presentation"}, .numCategories = 2, .parent = 3, .size = 3072 },
        { .id = 21, .name = "Video.mp4", .categories = {"Media", "Videos"}, .numCategories = 2, .parent = 34, .size = 6144 },
        { .id = 34, .name = "Folder2", .categories = {"Folder"}, .numCategories = 1, .parent = 3, .size = 0 },
        { .id = 55, .name = "Code.py", .categories = {"Programming"}, .numCategories = 1, .parent = -1, .size = 1536 },
        { .id = 89, .name = "Audio.mp3", .categories = {"Media", "Audio"}, .numCategories = 2, .parent = 34, .size = 2560 },
        { .id = 144, .name = "Spreadsheet2.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 2048 },
        { .id = 233, .name = "Folder3", .categories = {"Folder"}, .numCategories = 1, .parent = -1, .size = 4096 },
    };

    std::vector<std::string> expectedLeafFiles{
        "Audio.mp3",
        "Backup.zip",
        "Code.py",
        "Document.txt",
        "Image.jpg",
        "Presentation.pptx",
        "Spreadsheet.xlsx",
        "Spreadsheet2.xlsx",
        "Video.mp4"
    };
    std::vector<std::string> returnedLeafFiles = leafFiles(testFiles);
    std::sort(returnedLeafFiles.begin(), returnedLeafFiles.end());
    assert(expectedLeafFiles == returnedLeafFiles);

    std::vector<std::string> expectedCategories{"Documents", "Folder", "Media"};
    std::vector<std::string> returnedCategories = kLargestCategories(testFiles, 3);
    assert(expectedCategories == returnedCategories);

    assert(largestFileSize(testFiles) == 20992);

}