#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>
#include <windows.h>

// Запись данных в строку
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Поиск тега <title> в HTML
std::string extractTitle(std::string buffer) {
    size_t start = buffer.find("subdomain");
    size_t end = buffer.find("domain");
        if (start != std::string::npos && end != std::string::npos) {
            return buffer.substr(start, end);
        }
    return "Страница не найдена";
}

void teststr(char stroka[6])
{
    if (stroka[4] == '{')
    {
        stroka[3]++;
        stroka[4] = 'a';
    }
    if (stroka[3] == '{')
    {
        stroka[2]++;
        stroka[3] = 'a';
    }
    if (stroka[2] == '{')
    {
        stroka[1]++;
        stroka[2] = 'a';
    }
    if (stroka[1] == '{')
    {
        stroka[0]++;
        stroka[1] = 'a';
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    char str[6] = "caaaa";
    int count = 0;
    std::ofstream titlelist;
    std::ofstream lastnum;
    titlelist.open("words2.txt", std::ios::app);
    if (!titlelist.is_open())
    {
        std::cerr << "Не удалось открыть файл" << std::endl;
        return 1;
    }
    lastnum.open("last.txt");
    if (!lastnum.is_open())
    {
        std::cerr << "Не удалось открыть файл" << std::endl;
        return 1;
    }
    if (curl) {
        while (str[1] != 'n')
        {
            std::string url = "https://fragment.com/username/";
            url = url += str;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                std::string title = extractTitle(readBuffer);
                std::cout << url << std::endl;
                if (title == "Страница не найдена")
                {
                    titlelist << str << std::endl;
                }
            }
            else {
                std::cout << url << std::endl;
                std::cerr << "Ошибка curl: " << curl_easy_strerror(res) << std::endl;
            }
            if (count == 300)
            {
                count = 0;
                Sleep(20000);
            }
            readBuffer.clear();
            system("cls");
            count++;
            str[4]++;
            teststr(str);
        }
        curl_easy_cleanup(curl);
        lastnum << str;
    }
    curl_global_cleanup();
    lastnum.close();
    titlelist.close();

    return 0;
}
