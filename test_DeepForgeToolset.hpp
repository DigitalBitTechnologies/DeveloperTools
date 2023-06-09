#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "Progressbar.hpp"
#include <cctype>
#include <fstream>

// Проверка названия операционной системы и импортрование нужных библиотек для этой системы
#if defined(__linux__)
    #include <jsoncpp/json/json.h>
// cout << "Linux" << endl;
#elif __FreeBSD__
    #include <jsoncpp/json/json.h>
// cout << "FreeBSD" << endl;
#elif __APPLE__
    #include <jsoncpp/json/json.h>
// cout << "macOS" << endl;
#elif _WIN32
    #include <json/json.h>
    #include <conio.h>
    #include <Windows.h>
#endif

using namespace std;
using funct_t = void (*)(void);
using namespace Json;
// using namespace std::filesystem;


namespace AppInstaller {
    // Переменные
    // Value translate;
    // bool Install;
    // string InstallTools;
    // string LangReadySet;
    // string SelectPackages;
    // string Language = "English";
    // string OS_NAME;
    // string NameDistribution;
    // string Answer;
    // string new_sentence;
    // map<string, funct_t> Tools;
    // const string TrueVarious[3] = { "yes", "y", "1"};
    // const string KotlinUrl = "https://github.com/JetBrains/kotlin/releases/download/v1.8.22/kotlin-compiler-1.8.22.zip";
    // const string PHPUrl = "https://windows.php.net/downloads/releases/php-8.2.7-nts-Win32-vs16-x64.zip";
    // const string WgetUrl = "https://eternallybored.org/misc/wget/1.21.4/64/wget.exe";
    // string ProjectDir = std::filesystem::current_path().generic_string();
    // string KotlinDir = ProjectDir + "/Programms/Kotlin";
    // string PHPDir = ProjectDir + "/Programms/PHP";
    // string WgetDir = ProjectDir + "/Programms/Wget";
    // string NewKotlinDir = "C:\\Kotlin";
    // string NewPHPDir = "C:\\PHP";
    // string NewWgetDir = "C:\\Wget";
    // string AddKotlinPathScript = ProjectDir + "/Scripts/AddKotlinPath.ps1";
    // string AddPHPPathScript = ProjectDir + "/Scripts/AddPHPPathScript.ps1";
    // string AddWgetPathScript = ProjectDir + "/Scripts/AddWgetPath.ps1";
    // string TypeInstall = "open";
    // map<int, string> Languages{
    //     {1,"Python"},{2,"JavaScript"},{3,"C++"},{4,"Java"},
    //     {5,"Go"},{6,"Rust"},{7,"Ruby"},{8,"C"},
    //     {9,"C#"},{10,"PHP"},{11,"Kotlin"}
    // };
    // progressbar bar(100);
    // int TempPercentage = 0;

    // Функции
    string to_lower(string sentence)
    {
        string new_sentence = "";
        for(int i = 0;i < sentence.length();i++) {
            char ch = sentence[i];
            // cout << ch << endl;
            ch = tolower(ch);
            new_sentence += ch;
        }
        return new_sentence;
    }

    bool CheckAnswer(string answer)
    {
        bool status;
        // string Answer = to_lower(answer);

        string Answer = answer;
        for (int i = 0; i < TrueVarious->size(); i++) {
            if (Answer == TrueVarious[i] || Answer.empty() || Answer == "\n" || Answer == "да" || Answer == "ДА" || Answer == "Да") {
                status = true;
                break;
            }
        }
        return status;
    }
    size_t WriteData(void* ptr, size_t size, size_t nmemb, FILE* stream)
    {
        size_t WriteProcess = fwrite(ptr, size, nmemb, stream);
        return WriteProcess;
    }
    int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
    {
        // ensure that the file to be downloaded is not empty
        // because that would cause a division by zero error later on
        if (TotalToDownload <= 0.0) {
            return 0;
        }

        int percentage = static_cast<float>(NowDownloaded) / static_cast<float>(TotalToDownload) * 100;
        if (TempPercentage != percentage && TempPercentage <= 100) {
            bar.update();
            TempPercentage = percentage;
        }
        return 0;
    }
    int Download(string url, string dir)
    {
        try {
            string name = (url.substr(url.find_last_of("/")));
            string filename = dir + "/" + name.replace(name.find("/"), 1, "");
            CURL* curl = curl_easy_init();
            FILE* file = fopen(filename.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            CURLcode response = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(file);
            cout << "" << endl;
            return 200;
        }
        catch (string error) {
            return 502;
        }
    }
    void InstallGit()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallGit"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallGit"].asString() << endl;
                    system("winget install --id Git.Git -e --source winget");
                    cout << translate["InstalledGit"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallGit"].asString() << endl;
                    system("brew install --cask git");
                    cout << translate["InstalledGit"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallGit"].asString() << endl;
                    cout << translate["InstalledGit"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallGit"].asString() << endl;
                system("winget install --id Git.Git -e --source winget");
                cout << translate["InstalledGit"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallGit"].asString() << endl;
                system("brew install --cask git");
                cout << translate["InstalledGit"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallGit"].asString() << endl;
                cout << translate["InstalledGit"].asString() << endl;
            }
        }
    }
    void InstallVSCode()
    {   if (TypeInstall == "open") {
            cout << translate["WantInstallVSCode"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallVSCode"].asString() << endl;
                    system("winget install -e --id Microsoft.VisualStudioCode");
                    cout << translate["InstalledVSCode"] << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallVSCode"].asString() << endl;
                    system("brew install --cask visual-studio-code");
                    cout << translate["InstalledVSCode"] << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallVSCode"].asString() << endl;
                    system("snap install code --classic");
                    cout << translate["InstalledVSCode"] << endl;
                }
            }
        
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallVSCode"].asString() << endl;
                system("winget install -e --id Microsoft.VisualStudioCode");
                cout << translate["InstalledVSCode"] << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallVSCode"].asString() << endl;
                system("brew install --cask visual-studio-code");
                cout << translate["InstalledVSCode"] << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallVSCode"].asString() << endl;
                system("snap install code --classic");
                cout << translate["InstalledVSCode"] << endl;
            }
        }
    }
    void InstallWebStorm()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallWebStorm"].asString();
            cout << "Вы хотите установить JetBrains WebStorm (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallWebStorm"].asString() << endl;
                    system("winget install -e --id JetBrains.WebStorm");
                    cout << translate["InstalledWebStorm"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallWebStorm"].asString() << endl;
                    system("brew install --cask webstorm");
                    cout << translate["InstalledWebStorm"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallWebStorm"].asString() << endl;
                    system("snap install webstorm --classic");
                    cout << translate["InstalledWebStorm"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallWebStorm"].asString() << endl;
                system("winget install -e --id JetBrains.WebStorm");
                cout << translate["InstalledWebStorm"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallWebStorm"].asString() << endl;
                system("brew install --cask webstorm");
                cout << translate["InstalledWebStorm"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallWebStorm"].asString() << endl;
                system("snap install webstorm --classic");
                cout << translate["InstalledWebStorm"].asString() << endl;
            }
        }
    }
    void InstallDocker()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallDocker"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallDocker"].asString() << endl;
                    system("winget install -e --id Docker.DockerDesktop");
                    cout << translate["InstalledDocker"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallDocker"].asString() << endl;
                    system("brew install --cask docker");
                    cout << translate["InstalledDocker"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallDocker"].asString() << endl;
                    system("snap install docker");
                    cout << translate["InstalledDocker"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallDocker"].asString() << endl;
                system("winget install -e --id Docker.DockerDesktop");
                cout << translate["InstalledDocker"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallDocker"].asString() << endl;
                system("brew install --cask docker");
                cout << translate["InstalledDocker"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallDocker"].asString() << endl;
                system("snap install docker");
                cout << translate["InstalledDocker"].asString() << endl;
            }
        }
    }
    void InstallPostman()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallPostman"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallPostman"].asString() << endl;
                    system("winget install -e --id Postman.Postman");
                    cout << translate["InstalledPostman"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallPostman"].asString() << endl;
                    system("brew install --cask postman");
                    cout << translate["InstalledPostman"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallPostman"].asString() << endl;
                    system("snap install postman");
                    cout << translate["InstalledPostman"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallPostman"].asString() << endl;
                system("winget install -e --id Postman.Postman");
                cout << translate["InstalledPostman"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallPostman"].asString() << endl;
                system("brew install --cask postman");
                cout << translate["InstalledPostman"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallPostman"].asString() << endl;
                system("snap install postman");
                cout << translate["InstalledPostman"].asString() << endl;
            }
        }
    }
    void InstallRubyMine()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallRubyMine"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallRubyMine"].asString() << endl;
                    system("winget install -e --id JetBrains.RubyMine");
                    cout << translate["InstalledRubyMine"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallRubyMine"].asString() << endl;
                    system("brew install --cask rubymine");
                    cout << translate["InstalledRubyMine"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallRubyMine"].asString() << endl;
                    system("snap install rubymine --classic");
                    cout << translate["InstalledRubyMine"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallRubyMine"].asString() << endl;
                system("winget install -e --id JetBrains.RubyMine");
                cout << translate["InstalledRubyMine"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallRubyMine"].asString() << endl;
                system("brew install --cask rubymine");
                cout << translate["InstalledRubyMine"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallRubyMine"].asString() << endl;
                system("snap install rubymine --classic");
                cout << translate["InstalledRubyMine"].asString() << endl;
            }
        }
    }
    void InstallAqua()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallAqua"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallAqua"].asString() << endl;
                    system("winget install -e --id JetBrains.Aqua.EAP");
                    cout << translate["InstalledAqua"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallAqua"].asString() << endl;
                    cout << translate["InstalledAqua"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallAqua"].asString() << endl;
                    system("snap install jetbrains aqua --classic");
                    cout << translate["InstalledAqua"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallAqua"].asString() << endl;
                system("winget install -e --id JetBrains.Aqua.EAP");
                cout << translate["InstalledAqua"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallAqua"].asString() << endl;
                cout << translate["InstalledAqua"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallAqua"].asString() << endl;
                system("snap install jetbrains aqua --classic");
                cout << translate["InstalledAqua"].asString() << endl;
            }
        }
    }
    void InstallCLion()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallCLion"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallCLion"].asString() << endl;
                    system("winget install -e --id JetBrains.CLion");
                    cout << translate["InstalledCLion"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallCLion"].asString() << endl;
                    system("brew install --cask clion");
                    cout << translate["InstalledCLion"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallCLion"].asString() << endl;
                    system("snap install clion --classic");
                    cout << translate["InstalledCLion"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallCLion"].asString() << endl;
                system("winget install -e --id JetBrains.CLion");
                cout << translate["InstalledCLion"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallCLion"].asString() << endl;
                system("brew install --cask clion");
                cout << translate["InstalledCLion"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallCLion"].asString() << endl;
                system("snap install clion --classic");
                cout << translate["InstalledCLion"].asString() << endl;
            }
        }
    }
    void InstallDataGrip()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallDataGrip"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallDataGrip"].asString() << endl;
                    system("winget install -e --id JetBrains.DataGrip");
                    cout << translate["InstalledDataGrip"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallDataGrip"].asString() << endl;
                    system("brew install --cask datagrip");
                    cout << translate["InstalledDataGrip"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallDataGrip"].asString() << endl;
                    system("snap install datagrip --classic");
                    cout << translate["InstalledDataGrip"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallDataGrip"].asString() << endl;
                system("winget install -e --id JetBrains.DataGrip");
                cout << translate["InstalledDataGrip"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallDataGrip"].asString() << endl;
                system("brew install --cask datagrip");
                cout << translate["InstalledDataGrip"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallDataGrip"].asString() << endl;
                system("snap install datagrip --classic");
                cout << translate["InstalledDataGrip"].asString() << endl;
            }
        }
    }
    void InstallDataSpell()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallDataSpell"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallDataSpell"].asString() << endl;
                    system("winget install -e --id JetBrains.DataSpell");
                    cout << translate["InstalledDataSpell"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallDataSpell"].asString() << endl;
                    system("brew install --cask dataspell");
                    cout << translate["InstalledDataSpell"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallDataSpell"].asString() << endl;
                    system("snap install dataspell --classic");
                    cout << translate["InstalledDataSpell"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallDataSpell"].asString() << endl;
                system("winget install -e --id JetBrains.DataSpell");
                cout << translate["InstalledDataSpell"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallDataSpell"].asString() << endl;
                system("brew install --cask dataspell");
                cout << translate["InstalledDataSpell"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallDataSpell"].asString() << endl;
                system("snap install dataspell --classic");
                cout << translate["InstalledDataSpell"].asString() << endl;
            }
        }
    }
    void InstallFleet()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallFleet"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallFleet"].asString() << endl;
                    system("winget install -e --id JetBrains.FleetLauncher.Preview");
                    cout << translate["InstalledFleet"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallFleet"].asString() << endl;
                    system("brew install --cask fleet");
                    cout << translate["InstalledFleet"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallFleet"].asString() << endl;
                    cout << translate["InstalledFleet"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallFleet"].asString() << endl;
                system("winget install -e --id JetBrains.FleetLauncher.Preview");
                cout << translate["InstalledFleet"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallFleet"].asString() << endl;
                system("brew install --cask fleet");
                cout << translate["InstalledFleet"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallFleet"].asString() << endl;
                cout << translate["InstalledFleet"].asString() << endl;
            }
        }
    }
    void InstallGoLand()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallGoLand"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallGoLand"].asString() << endl;
                    system("winget install -e --id JetBrains.GoLand");
                    cout << translate["InstalledGoLand"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallGoLand"].asString() << endl;
                    system("brew install --cask goland");
                    cout << translate["InstalledGoLand"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallGoLand"].asString() << endl;
                    system("snap install goland --classic");
                    cout << translate["InstalledGoLand"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallGoLand"].asString() << endl;
                system("winget install -e --id JetBrains.GoLand");
                cout << translate["InstalledGoLand"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallGoLand"].asString() << endl;
                system("brew install --cask goland");
                cout << translate["InstalledGoLand"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallGoLand"].asString() << endl;
                system("snap install goland --classic");
                cout << translate["InstalledGoLand"].asString() << endl;
            }
        }
    }
    void InstallIntelliJCommunity()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallIntelliJIDEA Community"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallIntelliJIDEA Community"].asString() << endl;
                    system("winget install -e --id JetBrains.IntelliJIDEA.Community");
                    cout << translate["InstalledIntelliJIDEA Community"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallIntelliJIDEA Community"].asString() << endl;
                    system("brew install --cask intellij-idea-ce");
                    cout << translate["InstalledIntelliJIDEA Community"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallIntelliJIDEA Community"].asString() << endl;
                    system("snap install intellij-idea-community --classic");
                    cout << translate["InstalledIntelliJIDEA Community"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallIntelliJIDEA Community"].asString() << endl;
                system("winget install -e --id JetBrains.IntelliJIDEA.Community");
                cout << translate["InstalledIntelliJIDEA Community"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallIntelliJIDEA Community"].asString() << endl;
                system("brew install --cask intellij-idea-ce");
                cout << translate["InstalledIntelliJIDEA Community"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallIntelliJIDEA Community"].asString() << endl;
                system("snap install intellij-idea-community --classic");
                cout << translate["InstalledIntelliJIDEA Community"].asString() << endl;
            }
        }
    }
    void InstallIntelliJUltimate()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallIntelliJIDEA Ultimate"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallIntelliJIDEA Ultimate"].asString() << endl;
                    system("winget install -e --id JetBrains.IntelliJIDEA.Ultimate");
                    cout << translate["InstalledIntelliJIDEA Ultimate"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallIntelliJIDEA Ultimate"].asString() << endl;
                    system("brew install --cask intellij-idea");
                    cout << translate["InstalledIntelliJIDEA Ultimate"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallIntelliJIDEA Ultimate"].asString() << endl;
                    system("snap install intellij-idea-ultimate --classic");
                    cout << translate["InstalledIntelliJIDEA Ultimate"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallIntelliJIDEA Ultimate"].asString() << endl;
                system("winget install -e --id JetBrains.IntelliJIDEA.Ultimate");
                cout << translate["InstalledIntelliJIDEA Ultimate"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallIntelliJIDEA Ultimate"].asString() << endl;
                system("brew install --cask intellij-idea");
                cout << translate["InstalledIntelliJIDEA Ultimate"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallIntelliJIDEA Ultimate"].asString() << endl;
                system("snap install intellij-idea-ultimate --classic");
                cout << translate["InstalledIntelliJIDEA Ultimate"].asString() << endl;
            }
        }
    }
    void InstallReSharper()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallReSharper"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallReSharper"].asString() << endl;
                    system("winget install -e --id JetBrains.ReSharper");
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["ReSharper_MacOS"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["ReSharper_Linux"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallReSharper"].asString() << endl;
                system("winget install -e --id JetBrains.ReSharper");
            }
            else if (OS_NAME == "macOS") {
                cout << translate["ReSharper_MacOS"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["ReSharper_Linux"].asString() << endl;
            }
        }
    }
    void InstallRider()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallRider"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallRider"].asString() << endl;
                    system("winget install -e --id JetBrains.Rider");
                    cout << translate["InstalledRider"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallRider"].asString() << endl;
                    system("brew install --cask rider");
                    cout << translate["InstalledRider"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallRider"].asString() << endl;
                    system("snap install rider --classic");
                    cout << translate["InstalledRider"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallRider"].asString() << endl;
                system("winget install -e --id JetBrains.Rider");
                cout << translate["InstalledRider"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallRider"].asString() << endl;
                system("brew install --cask rider");
                cout << translate["InstalledRider"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallRider"].asString() << endl;
                system("snap install rider --classic");
                cout << translate["InstalledRider"].asString() << endl;
            }
        }
    }
    void InstallPhpStorm()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallPHPStorm"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallPHPStorm"].asString() << endl;
                    system("winget install -e --id JetBrains.PHPStorm");
                    cout << translate["InstalledPHPStorm"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallPHPStorm"].asString() << endl;
                    system("brew install --cask phpstorm");
                    cout << translate["InstalledPHPStorm"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallPHPStorm"].asString() << endl;
                    system("snap install phpstorm --classic");
                    cout << translate["InstalledPHPStorm"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallPHPStorm"].asString() << endl;
                system("winget install -e --id JetBrains.PHPStorm");
                cout << translate["InstalledPHPStorm"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallPHPStorm"].asString() << endl;
                system("brew install --cask phpstorm");
                cout << translate["InstalledPHPStorm"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallPHPStorm"].asString() << endl;
                system("snap install phpstorm --classic");
                cout << translate["InstalledPHPStorm"].asString() << endl;
            }
        }
    }
    void InstalldotUltimate()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstalldotUltimate"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstalldotUltimate"].asString() << endl;
                    system("winget install -e --id JetBrains.dotUltimate");
                    cout << translate["InstalleddotUltimate"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["dotUltimate_MacOS"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["dotUltimate_Linux"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstalldotUltimate"].asString() << endl;
                system("winget install -e --id JetBrains.dotUltimate");
                cout << translate["InstalleddotUltimate"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["dotUltimate_MacOS"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["dotUltimate_Linux"].asString() << endl;
            }
        }
    }
    void InstallSpace()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallSpace"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallSpace"].asString() << endl;
                    system("winget install -e --id JetBrains.Space");
                    cout << translate["InstalledSpace"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallSpace"].asString() << endl;
                    system("brew install --cask jetbrains-space");
                    cout << translate["InstalledSpace"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallSpace"].asString() << endl;
                    system("snap install space");
                    cout << translate["InstalledSpace"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallSpace"].asString() << endl;
                system("winget install -e --id JetBrains.Space");
                cout << translate["InstalledSpace"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallSpace"].asString() << endl;
                system("brew install --cask jetbrains-space");
                cout << translate["InstalledSpace"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallSpace"].asString() << endl;
                system("snap install space");
                cout << translate["InstalledSpace"].asString() << endl;
            }
        }
    }
    void InstallToolBox()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallToolbox"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallToolbox"].asString() << endl;
                    system("winget install -e --id JetBrains.Toolbox");
                    cout << translate["InstalledToolbox"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallToolbox"].asString() << endl;
                    system("brew install --cask jetbrains-toolbox");
                    cout << translate["InstalledToolbox"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallToolbox"].asString() << endl;
                    cout << translate["InstalledToolbox"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallToolbox"].asString() << endl;
                system("winget install -e --id JetBrains.Toolbox");
                cout << translate["InstalledToolbox"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallToolbox"].asString() << endl;
                system("brew install --cask jetbrains-toolbox");
                cout << translate["InstalledToolbox"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallToolbox"].asString() << endl;
                cout << translate["InstalledToolbox"].asString() << endl;
            }
        }
    }
    void InstallPostgresql()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallPostgreSQL"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallPostgreSQL"].asString() << endl;
                    system("winget install -e --id PostgreSQL.PostgreSQL");
                    system("winget install -e --id PostgreSQL.pgAdmin");
                    cout << translate["InstalledPostgreSQL"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallPostgreSQL"].asString() << endl;
                    system("brew install postgresql@14");
                    system("brew install --cask pgadmin4");
                    cout << translate["InstalledPostgreSQL"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallPostgreSQL"].asString() << endl;
                    system("snap install postgresql");
                    cout << translate["InstalledPostgreSQL"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallPostgreSQL"].asString() << endl;
                system("winget install -e --id PostgreSQL.PostgreSQL");
                system("winget install -e --id PostgreSQL.pgAdmin");
                cout << translate["InstalledPostgreSQL"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallPostgreSQL"].asString() << endl;
                system("brew install postgresql@14");
                system("brew install --cask pgadmin4");
                cout << translate["InstalledPostgreSQL"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallPostgreSQL"].asString() << endl;
                system("snap install postgresql");
                cout << translate["InstalledPostgreSQL"].asString() << endl;
            }
        }
    }
    void InstallCLink()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallCLink"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallCLink"].asString() << endl;
                    system("winget install -e --id chrisant996.Clink");
                    cout << translate["InstalledCLink"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["CLink_MacOS"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["CLink_Linux"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallCLink"].asString() << endl;
                system("winget install -e --id chrisant996.Clink");
                cout << translate["InstalledCLink"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["CLink_MacOS"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["CLink_Linux"].asString() << endl;
            }
        }
    }
    void InstallNgrok()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallCLink"].asString();
            cout << "Вы хотите установить Ngrok (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallNgrok"].asString() << endl;
                    system("winget install -e --id Ngrok.Ngrok");
                    cout << translate["InstalledNgrok"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallNgrok"].asString() << endl;
                    system("brew install --cask ngrok");
                    cout << translate["InstalledNgrok"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallNgrok"].asString() << endl;
                    system("snap install ngrok");
                    cout << translate["InstalledNgrok"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallNgrok"].asString() << endl;
                system("winget install -e --id Ngrok.Ngrok");
                cout << translate["InstalledNgrok"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallNgrok"].asString() << endl;
                system("brew install --cask ngrok");
                cout << translate["InstalledNgrok"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallNgrok"].asString() << endl;
                system("snap install ngrok");
                cout << translate["InstalledNgrok"].asString() << endl;
            }
        }
    }
    void InstallWget()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallWget"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallWget"].asString() << endl;
                    system("winget install -e --id JernejSimoncic.Wget");
                    cout << translate["InstalledWget"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallWget"].asString() << endl;
                    system("brew install wget");
                    cout << translate["InstalledWget"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallWget"].asString() << endl;
                    cout << translate["InstalledWget"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallWget"].asString() << endl;
                system("winget install -e --id JernejSimoncic.Wget");
                cout << translate["InstalledWget"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallWget"].asString() << endl;
                system("brew install wget");
                cout << translate["InstalledWget"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallWget"].asString() << endl;
                cout << translate["InstalledWget"].asString() << endl;
            }
        }
    }
    void InstallSublimeText()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallSublimeText"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallSublimeText"].asString() << endl;
                    system("winget install -e --id SublimeHQ.SublimeText.4");
                    cout << translate["InstalledSublimeText"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstalledSublimeText"].asString() << endl;
                    system("brew install --cask sublime-text");
                    cout << translate["InstalledSublimeText"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstalledSublimeText"].asString() << endl;
                    system("snap install sublime-text --classic");
                    cout << translate["InstalledSublimeText"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstalledSublimeText"].asString() << endl;
                system("winget install -e --id SublimeHQ.SublimeText.4");
                cout << translate["InstalledSublimeText"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstalledSublimeText"].asString() << endl;
                system("brew install --cask sublime-text");
                cout << translate["InstalledSublimeText"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstalledSublimeText"].asString() << endl;
                system("snap install sublime-text --classic");
                cout << translate["InstalledSublimeText"].asString() << endl;
            }
        }
    }
    void InstallPyCharmCommunity()
    {
        if (TypeInstall == "open") {
            cout << translate["WantInstallPyCharm_CE"].asString();
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << translate["InstallPyCharm_CE"].asString() << endl;
                    system("winget install -e --id JetBrains.PyCharm.Community");
                    cout << translate["InstalledPyCharm_CE"].asString() << endl;
                }
                else if (OS_NAME == "macOS") {
                    cout << translate["InstallPyCharm_CE"].asString() << endl;
                    system("brew install --cask pycharm-ce");
                    cout << translate["InstalledPyCharm_CE"].asString() << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << translate["InstallPyCharm_CE"].asString() << endl;
                    system("snap install pycharm-community --classic");
                    cout << translate["InstalledPyCharm_CE"].asString() << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << translate["InstallPyCharm_CE"].asString() << endl;
                system("winget install -e --id JetBrains.PyCharm.Community");
                cout << translate["InstalledPyCharm_CE"].asString() << endl;
            }
            else if (OS_NAME == "macOS") {
                cout << translate["InstallPyCharm_CE"].asString() << endl;
                system("brew install --cask pycharm-ce");
                cout << translate["InstalledPyCharm_CE"].asString() << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << translate["InstallPyCharm_CE"].asString() << endl;
                system("snap install pycharm-community --classic");
                cout << translate["InstalledPyCharm_CE"].asString() << endl;
            }
        }
    }
    void InstallPyCharmProffessional()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить JetBrains PyCharm Professional (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка JetBrains PyCharm Professional ..." << endl;
                    system("winget install -e --id JetBrains.PyCharm.Professional");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка JetBrains PyCharm ..." << endl;
                    system("brew install --cask pycharm");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка JetBrains PyCharm Professional ..." << endl;
                    system("snap install pycharm-professional --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                system("winget install -e --id JetBrains.PyCharm.Professional");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка JetBrains PyCharm ..." << endl;
                system("brew install --cask pycharm");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка JetBrains PyCharm Professional ..." << endl;
                system("snap install pycharm-professional --classic");
            }
        }
    }
    void InstallPowerToys()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Microsoft PowerToys (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Microsoft PowerToys ..." << endl;
                    system("winget install -e --id Microsoft.PowerToys");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Microsoft PowerToys не поддерживается на MacOS" << endl;
                }
                else if (OS_NAME == "Linux") {
                    cout << "Microsoft PowerToys не поддерживается на Linux" << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Microsoft PowerToys ..." << endl;
                system("winget install -e --id Microsoft.PowerToys");
            }
            else if (OS_NAME == "macOS") {
                cout << "Microsoft PowerToys не поддерживается на MacOS" << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << "Microsoft PowerToys не поддерживается на Linux" << endl;
            }
        }
    }
    void InstallDiscord()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Discord (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Discord ..." << endl;
                    system("winget install -e --id Discord.Discord");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Discord ..." << endl;
                    system("brew install --cask discord");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Discord ..." << endl;
                    system("snap install discord");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Discord ..." << endl;
                system("winget install -e --id Discord.Discord");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Discord ..." << endl;
                system("brew install --cask discord");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Discord ..." << endl;
                system("snap install discord");
            }
        }
    }
    void InstallTelegram()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Telegram (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Telegram ..." << endl;
                    system("winget install -e --id Telegram.TelegramDesktop");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Telegram ..." << endl;
                    system("brew install --cask telegram");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Telegram ..." << endl;
                    system("snap install telegram-desktop");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Telegram ..." << endl;
                system("winget install -e --id Telegram.TelegramDesktop");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Telegram ..." << endl;
                system("brew install --cask telegram");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Telegram ..." << endl;
                system("snap install telegram-desktop");
            }
        }
    }
    void InstallVNCServer()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить RealVNC Server (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка RealVNC Server ..." << endl;
                    system("winget install -e --id RealVNC.VNCServer");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка RealVNC Server ..." << endl;
                    system("brew install --cask vn-server");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка RealVNC Server ..." << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка RealVNC Server ..." << endl;
                system("winget install -e --id RealVNC.VNCServer");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка RealVNC Server ..." << endl;
                system("brew install --cask vn-server");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка RealVNC Server ..." << endl;
            }
        }
    }
    void InstallVNCViewer()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить RealVNC Viewer (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка RealVNC Viewer ..." << endl;
                    system("winget install -e --id RealVNC.VNCViewer");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка RealVNC Viewer ..." << endl;
                    system("brew install --cask vn-viewer");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка RealVNC Viewer ..." << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка RealVNC Viewer ..." << endl;
                system("winget install -e --id RealVNC.VNCViewer");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка RealVNC Viewer ..." << endl;
                system("brew install --cask vn-viewer");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка RealVNC Viewer ..." << endl;
            }
        }
    }
    void InstallMongoDBCompass()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить MongoDB Compass (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка MongoDB Compass ..." << endl;
                    system("winget install -e --id MongoDB.Compass.Full");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка MongoDB Compass ..." << endl;
                    system("brew install --cask mongodb-compass");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка MongoDB Compass ..." << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка MongoDB Compass ..." << endl;
                system("winget install -e --id MongoDB.Compass.Full");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка MongoDB Compass ..." << endl;
                system("brew install --cask mongodb-compass");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка MongoDB Compass ..." << endl;
            }
        }
    }
    void InstallMongoDB()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить MongoDB Server (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка MongoDB Server ..." << endl;
                    system("winget install -e --id MongoDB.Server");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка MongoDB Server ..." << endl;
                    system("brew install mongodb-community");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка MongoDB Server ..." << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка MongoDB Server ..." << endl;
                system("winget install -e --id MongoDB.Server");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка MongoDB Server ..." << endl;
                system("brew install mongodb-community");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка MongoDB Server ..." << endl;
            }
        }
    }
    void InstallMongoDBAtlas()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить MongoDB MongoDBAtlasCLI (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка MongoDB Atlas ..." << endl;
                    system("winget install -e --id MongoDB.MongoDBAtlasCLI");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка MongoDB Atlas ..." << endl;
                    system("brew install mongodb-atlas-cli");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка MongoDB Atlas ..." << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка MongoDB Atlas ..." << endl;
                system("winget install -e --id MongoDB.MongoDBAtlasCLI");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка MongoDB Atlas ..." << endl;
                system("brew install mongodb-atlas-cli");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка MongoDB Atlas ..." << endl;
            }
        }
    }
    void InstallNodeJS()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить NodeJS (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка NodeJS ..." << endl;
                    system("winget install -e --id OpenJS.NodeJS");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка NodeJS ..." << endl;
                    system("brew install node");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка NodeJS ..." << endl;
                    system("snap install node --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка NodeJS ..." << endl;
                system("winget install -e --id OpenJS.NodeJS");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка NodeJS ..." << endl;
                system("brew install node");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка NodeJS ..." << endl;
                system("snap install node --classic");
            }
        }
    }
    void InstallGoLang()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить GoLang 1.18 (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка GoLang ..." << endl;
                    system("winget install -e --id GoLang.Go.1.18");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка GoLang ..." << endl;
                    system("brew install go");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка GoLang ..." << endl;
                    system("snap install go --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка GoLang ..." << endl;
                system("winget install -e --id GoLang.Go.1.18");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка GoLang ..." << endl;
                system("brew install go");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка GoLang ..." << endl;
                system("snap install go --classic");
            }
        }
    }
    void InstallPython3_9()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Python 3.9 (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка 🐍 python 3.9..." << endl;
                    system("winget install -e --id Python.Python.3.9");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка 🐍 python 3.9..." << endl;
                    system("brew install python@3.9");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка 🐍 python 3.9..." << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка 🐍 python 3.9..." << endl;
                system("winget install -e --id Python.Python.3.9");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка 🐍 python 3.9..." << endl;
                system("brew install python@3.9");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка 🐍 python 3.9..." << endl;
            }
        }
    }
    void InstallPython3_10()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Python 3.10 (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка 🐍 python 3.10..." << endl;
                    system("winget install -e --id Python.Python.3.10");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка 🐍 python 3.10..." << endl;
                    system("brew install python@3.10");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка 🐍 python 3.10..." << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                    cout << "Установка 🐍 python 3.10..." << endl;
                    system("winget install -e --id Python.Python.3.10");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка 🐍 python 3.10..." << endl;
                    system("brew install python@3.10");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка 🐍 python 3.10..." << endl;
                }
        }
    }
    void InstallPython3_11()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Python 3.11 (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка 🐍 python 3.11..." << endl;
                    system("winget install -e --id Python.Python.3.11");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка 🐍 python 3.11..." << endl;
                    system("brew install python@3.11");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка 🐍 python 3.11..." << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка 🐍 python 3.11..." << endl;
                system("winget install -e --id Python.Python.3.11");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка 🐍 python 3.11..." << endl;
                system("brew install python@3.11");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка 🐍 python 3.11..." << endl;
            }
        }
    }
    void InstallJDK_19()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить JDK 19 (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка JDK 19 ..." << endl;
                    system("winget install -e --id Oracle.JDK.19");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка JDK 19 ..." << endl;
                    system("brew install openjdk@19");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка OpenJDK ..." << endl;
                    system("snap install openjdk");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка JDK 19 ..." << endl;
                system("winget install -e --id Oracle.JDK.19");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка JDK 19 ..." << endl;
                system("brew install openjdk@19");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка OpenJDK ..." << endl;
                system("snap install openjdk");
            }
        }
    }
    void InstallJDK_18()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить JDK 18 (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка JDK 18 ..." << endl;
                    system("winget install -e --id Oracle.JDK.18");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка JDK 18 ..." << endl;
                    system("brew install openjdk@18");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка OpenJDK ..." << endl;
                    system("snap install openjdk");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка JDK 18 ..." << endl;
                system("winget install -e --id Oracle.JDK.18");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка JDK 18 ..." << endl;
                system("brew install openjdk@18");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка OpenJDK ..." << endl;
                system("snap install openjdk");
            }
        }
    }
    void InstallRust()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Rust (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Rust ..." << endl;
                    system("winget install -e --id Rustlang.Rust.MSVC");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Rust ..." << endl;
                    system("brew install rust");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Rust ..." << endl;
                    system("snap install rustup --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Rust ..." << endl;
                system("winget install -e --id Rustlang.Rust.MSVC");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Rust ..." << endl;
                system("brew install rust");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Rust ..." << endl;
                system("snap install rustup --classic");
            }
        }
    }
    void InstallNetFramework()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Microsoft DotNet Framework (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Microsoft DotNet Framework ..." << endl;
                    system("winget install -e --id Microsoft.DotNet.Framework.DeveloperPack_4");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Microsoft DotNet Framework ..." << endl;
                    system("brew install dotnet");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Microsoft .NET Core SDK ..." << endl;
                    system("snap install dotnet-sdk --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Microsoft DotNet Framework ..." << endl;
                system("winget install -e --id Microsoft.DotNet.Framework.DeveloperPack_4");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Microsoft DotNet Framework ..." << endl;
                system("brew install dotnet");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Microsoft .NET Core SDK ..." << endl;
                system("snap install dotnet-sdk --classic");
            }
        }
    }
    void InstallRuby()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Ruby (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Ruby ..." << endl;
                    system("winget install -e --id RubyInstallerTeam.Ruby.3.1");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Ruby ..." << endl;
                    system("brew install ruby");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Ruby ..." << endl;
                    system("snap install ruby --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Ruby ..." << endl;
                system("winget install -e --id RubyInstallerTeam.Ruby.3.1");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Ruby ..." << endl;
                system("brew install ruby");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Ruby ..." << endl;
                system("snap install ruby --classic");
            }
        }
    }
    void InstallMSYS2()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить MSYS2 (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка MSYS2 ..." << endl;
                    system("winget install -e --id MSYS2.MSYS2");
                }
            }
            else if (OS_NAME == "macOS") {
                cout << "MSYS2 не поддерживается на MacOS" << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << "MSYS2 не поддерживается на MacOS" << endl;
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка MSYS2 ..." << endl;
                system("winget install -e --id MSYS2.MSYS2");
            }
            else if (OS_NAME == "macOS") {
                cout << "MSYS2 не поддерживается на MacOS" << endl;
            }
            else if (OS_NAME == "Linux") {
                cout << "MSYS2 не поддерживается на MacOS" << endl;
            }
        }
    }
    void InstallNuget()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Microsoft NuGet (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Nuget ..." << endl;
                    system("winget install -e --id Microsoft.NuGet");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Nuget ..." << endl;
                    system("brew install nuget");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Nuget ..." << endl;
                    cout << "Nuget не поддерживается на Linux" << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Nuget ..." << endl;
                system("winget install -e --id Microsoft.NuGet");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Nuget ..." << endl;
                system("brew install nuget");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Nuget ..." << endl;
                cout << "Nuget не поддерживается на Linux" << endl;
            }
        }
    }
    void InstallGitHubDesktop()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить GitHub Desktop (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка GitHub Desktop ..." << endl;
                    system("winget install -e --id GitHub.GitHubDesktop");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка GitHub Desktop ..." << endl;
                    system("brew install --cask github");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка GitHub Desktop ..." << endl;
                    system("snap install github-gui");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка GitHub Desktop ..."<< endl;
                system("winget install -e --id GitHub.GitHubDesktop");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка GitHub Desktop ..." << endl;
                system("brew install --cask github");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка GitHub Desktop ..." << endl;
                system("snap install github-gui");
            }
        }
    }
    void InstallGitHubCLi()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить GitHub CLI (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка GitHub CLI ..." << endl;
                    system("winget install -e --id GitHub.cli");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка GitHub CLI ..." << endl;
                    system("brew install gh");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка GitHub CLI ..." << endl;
                    system("snap install gh");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка GitHub CLI ..." << endl;
                system("winget install -e --id GitHub.cli");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка GitHub CLI ..." << endl;
                system("brew install gh");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка GitHub CLI ..." << endl;
                system("snap install gh");
            }
        }
    }
    void InstallKubernetes()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Kubernetes (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Kubernetes ..." << endl;
                    system("winget install -e --id Kubernetes.kubectl");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Kubernetes ..." << endl;
                    system("brew install kubernetes-cli");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Kubernetes ..." << endl;
                    system("snap install kubectl --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Kubernetes ..." << endl;
                system("winget install -e --id Kubernetes.kubectl");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Kubernetes ..." << endl;
                system("brew install kubernetes-cli");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Kubernetes ..." << endl;
                system("snap install kubectl --classic");
            }
        }
    }
    void InstallVisualStudioCommunity()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Visual Studio Community (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Visual Studio Community ..." << endl;
                    system("winget install -e --id Microsoft.VisualStudio.2022.Community.Preview");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Visual Studio Community ..." << endl;
                    system("brew install --cask visual-studio");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Visual Studio Community ..." << endl;
                    cout << "Visual Studio Community не поддерживается на Linux" << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Visual Studio Community ..." << endl;
                system("winget install -e --id Microsoft.VisualStudio.2022.Community.Preview");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Visual Studio Community ..." << endl;
                system("brew install --cask visual-studio");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Visual Studio Community ..." << endl;
                cout << "Visual Studio Community не поддерживается на Linux" << endl;
            }
        }
    }

    void InstallVisualStudioProffessional()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Visual Studio Proffessional (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Visual Studio Proffessional ..." << endl;
                    system("winget install -e --id Microsoft.VisualStudio.2022.Professional.Preview");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Visual Studio Community ..." << endl;
                    system("brew install --cask visual-studio");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Visual Studio Proffessional не поддерживается на Linux" << endl;
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Visual Studio Proffessional ..." << endl;
                system("winget install -e --id Microsoft.VisualStudio.2022.Professional.Preview");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Visual Studio Community ..." << endl;
                system("brew install --cask visual-studio");
            }
            else if (OS_NAME == "Linux") {
                cout << "Visual Studio Proffessional не поддерживается на Linux" << endl;
            }
        }
    }
    void InstallSlack()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Slack (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Slack ..." << endl;
                    system("winget install -e --id SlackTechnologies.Slack");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Slack ..." << endl;
                    system("brew install --cask slack");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Slack ..." << endl;
                    system("snap install slack");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Slack ..." << endl;
                system("winget install -e --id SlackTechnologies.Slack");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Slack ..." << endl;
                system("brew install --cask slack");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Slack ..." << endl;
                system("snap install slack");
            }
        }
    }
    void InstallVim()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Vim (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Vim ..." << endl;
                    system("winget install -e --id vim.vim");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Vim ..." << endl;
                    system("brew install vim");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Vim ..." << endl;
                    system("snap install vim-editor --beta");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Vim ..." << endl;
                system("winget install -e --id vim.vim");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Vim ..." << endl;
                system("brew install vim");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Vim ..." << endl;
                system("snap install vim-editor --beta");
            }
        }
    }
    void InstallNeoVim()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить NeoVim (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка NeoVim ..." << endl;
                    system("winget install -e --id Neovim.Neovim");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка NeoVim ..." << endl;
                    system("brew install neovim");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка NeoVim ..." << endl;
                    system("snap install nvim --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка NeoVim ..." << endl;
                system("winget install -e --id Neovim.Neovim");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка NeoVim ..." << endl;
                system("brew install neovim");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка NeoVim ..." << endl;
                system("snap install nvim --classic");
            }
        }
    }
    void InstallGoogleChrome()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Google Chrome (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Google Chrome ..." << endl;
                    system("winget install -e --id Google.Chrome");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Google Chrome ..." << endl;
                    system("brew install --cask google-chrome");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Chromium ..." << endl;
                    system("snap install chromium");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Google Chrome ..." << endl;
                system("winget install -e --id Google.Chrome");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Google Chrome ..." << endl;
                system("brew install --cask google-chrome");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Chromium ..." << endl;
                system("snap install chromium");
            }
        }
    }
    void InstallAndroidStudio()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Android Studio (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Android Studio ..." << endl;
                    system("winget install -e --id Google.AndroidStudio");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Android Studio ..." << endl;
                    system("brew install --cask android-studio");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Android Studio ..." << endl;
                    system("snap install android-studio --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Android Studio ..." << endl;
                system("winget install -e --id Google.AndroidStudio");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Android Studio ..." << endl;
                system("brew install --cask android-studio");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Android Studio ..." << endl;
                system("snap install android-studio --classic");
            }
        }
    }
    void InstallEclipse()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Eclipse (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    cout << "Установка Eclipse ..." << endl;
                    system("winget install -e --id EclipseAdoptium.Temurin.18.JDK");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Eclipse ..." << endl;
                    system("brew install --cask eclipse-ide");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Eclipse ..." << endl;
                    system("snap install eclipse --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Eclipse ..." << endl;
                system("winget install -e --id EclipseAdoptium.Temurin.18.JDK");
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Eclipse ..." << endl;
                system("brew install --cask eclipse-ide");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Eclipse ..." << endl;
                system("snap install eclipse --classic");
            }
        }
    }
    void InstallKotlin()
    {
        if (TypeInstall == "open") {
            cout << "Вы хотите установить Kotlin (по умолчанию - да)?";
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    try {
                        cout << "Установка Kotlin ..." << endl;
                        int response = Download(KotlinUrl, KotlinDir);
                        
                        filesystem::create_directory(NewKotlinDir);
                        string Command = "tar -xf" + ProjectDir + "/kotlin-compiler-1.8.22.zip " + "--directory " + NewKotlinDir;
                        system(Command.c_str());
                        string AddPathCommand = "start powershell.exe -file " + AddKotlinPathScript;
                        system("powershell.exe  C?\\Users\\Blackflame576\\Documents\\Blackflame576\\DigitalBit\\DeveloperTools\\Scripts\\AddKotlinPath.ps1");
                        system(AddPathCommand.c_str());
                        cout << AddKotlinPathScript << endl;
                        switch (response) {
                            case 200:
                                cout << "✅ Kotlin успешно скачан" << endl;
                                break;
                            case 502:
                                cout << "❌ Произошла ошибка при попытке скачать Kotlin" << endl;
                                break;
                        }
                        // std??filesystem??copy(KotlinDir,NewKotlinDir,std??filesystem??copy_options??recursive);
                        // auto ProjectDir = std??filesystem??current_path();
                        // cout << "Current Path?" << ProjectDir << endl;
                        // filesystem??create_directory();
                        // filesystem??copy();
                        // filesystem??remove_all();
                    }
                    catch (std::filesystem::filesystem_error& error) {
                        cout << error.what() << endl;
                    }
                    // system("winget install -e --id EclipseAdoptium.Temurin.18.JDK");
                }
                else if (OS_NAME == "macOS") {
                    cout << "Установка Kotlin ..." << endl;
                    system("brew install kotlin");
                }
                else if (OS_NAME == "Linux") {
                    cout << "Установка Kotlin ..." << endl;
                    system("snap install kotlin --classic");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                cout << "Установка Kotlin ..." << endl;
                int response = Download(KotlinUrl, KotlinDir);
                filesystem::create_directory(NewKotlinDir);
                string Command = "tar -xf" + ProjectDir + "/kotlin-compiler-1.8.22.zip " + "--directory " + NewKotlinDir;
                system(Command.c_str());
                string AddPathCommand = "powershell.exe -file " + AddKotlinPathScript;
                system(AddPathCommand.c_str());
                switch (response) {
                    case 200:
                        cout << "✅ Kotlin успешно скачан" << endl;
                        break;
                    case 502:
                        cout << "❌ Произошла ошибка при попытке скачать Kotlin" << endl;
                        break;
                }
            }
            else if (OS_NAME == "macOS") {
                cout << "Установка Kotlin ..." << endl;
                system("brew install kotlin");
            }
            else if (OS_NAME == "Linux") {
                cout << "Установка Kotlin ..." << endl;
                system("snap install kotlin --classic");
            }
        }
    }
    
    void InstallNetSDK_7() {
        if (TypeInstall == "open") {
            if (Language == "Russian") {
                cout << "Вы хотите установить Microsoft .NET SDK 7.0 (по умолчанию - да)?";
            }
            else {
                cout << "Do you want to install Microsoft .NET SDK 7.0 (default - yes)?";
            }
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    if (Language == "Russian") {
                        cout << "Установка .NET SDK 7.0 ..." << endl;
                    }
                    else {
                        cout << "Installing .NET SDK 7.0 ..." << endl;
                    }
                    system("winget install -e --id Microsoft.DotNet.SDK.7");
                }
                else if (OS_NAME == "macOS") {
                    if (Language == "Russian") {
                        cout << "Установка .NET SDK 7.0 ..." << endl;
                    }
                    else {
                        cout << "Installing .NET SDK 7.0 ..." << endl;
                    }
                    system("brew install dotnet");
                }
                else if (OS_NAME == "Linux") {
                    if (Language == "Russian") {
                        cout << "Установка .NET SDK 7.0 ..." << endl;
                    }
                    else {
                        cout << "Installing .NET SDK 7.0 ..." << endl;
                    }
                    system("sudo snap install dotnet-sdk --classic --channel=7.0");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                if (Language == "Russian") {
                    cout << "Установка .NET SDK 7.0 ..." << endl;
                }
                else {
                    cout << "Installing .NET SDK 7.0 ..." << endl;
                }
                system("winget install -e --id Microsoft.DotNet.SDK.7");
            }
            else if (OS_NAME == "macOS") {
                if (Language == "Russian") {
                    cout << "Установка .NET SDK 7.0 ..." << endl;
                }
                else {
                    cout << "Installing .NET SDK 7.0 ..." << endl;
                }
                system("brew install dotnet");
            }
            else if (OS_NAME == "Linux") {
                if (Language == "Russian") {
                    cout << "Установка .NET SDK 7.0 ..." << endl;
                }
                else {
                    cout << "Installing .NET SDK 7.0 ..." << endl;
                }
                system("sudo snap install dotnet-sdk --classic --channel=7.0");
            }
        }
    }

    void InstallNetSDK_6() {
        if (TypeInstall == "open") {
            if (Language == "Russian") {
                cout << "Вы хотите установить Microsoft .NET SDK 6.0 (по умолчанию - да)?";
            }
            else {
                cout << "Do you want to install Microsoft .NET SDK 6.0 (default - yes)?";
            }
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    if (Language == "Russian") {
                        cout << "Установка .NET SDK 6.0 ..." << endl;
                    }
                    else {
                        cout << "Installing .NET SDK 6.0 ..." << endl;
                    }
                    system("winget install -e --id Microsoft.DotNet.SDK.6");
                }
                else if (OS_NAME == "macOS") {
                    if (Language == "Russian") {
                        cout << "Установка .NET SDK 6.0 ..." << endl;
                    }
                    else {
                        cout << "Installing .NET SDK 6.0 ..." << endl;
                    }
                    system("brew install dotnet@6");
                }
                else if (OS_NAME == "Linux") {
                    if (Language == "Russian") {
                        cout << "Установка .NET SDK 6.0 ..." << endl;
                    }
                    else {
                        cout << "Installing .NET SDK 6.0 ..." << endl;
                    }
                    system("sudo snap install dotnet-sdk --classic --channel=6.0");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                if (Language == "Russian") {
                    cout << "Установка .NET SDK 6.0 ..." << endl;
                }
                else {
                    cout << "Installing .NET SDK 6.0 ..." << endl;
                }
                system("winget install -e --id Microsoft.DotNet.SDK.6");
            }
            else if (OS_NAME == "macOS") {
                if (Language == "Russian") {
                    cout << "Установка .NET SDK 6.0 ..." << endl;
                }
                else {
                    cout << "Installing .NET SDK 6.0 ..." << endl;
                }
                system("brew install dotnet@6");
            }
            else if (OS_NAME == "Linux") {
                if (Language == "Russian") {
                    cout << "Установка .NET SDK 6.0 ..." << endl;
                }
                else {
                    cout << "Installing .NET SDK 6.0 ..." << endl;
                }
                system("sudo snap install dotnet-sdk --classic --channel=6.0");
            }
        }
    }

    void InstallNetRuntime_7() {
        if (TypeInstall == "open") {
            if (Language == "Russian") {
                cout << "Вы хотите установить Microsoft .NET Runtime 7.0 (по умолчанию - да)?";
            }
            else {
                cout << "Do you want to install Microsoft .NET Runtime 7.0 (default - yes)?";
            }
            getline(cin, Answer);
            Install = CheckAnswer(Answer);
            if (Install == true) {
                if (OS_NAME == "Windows") {
                    if (Language == "Russian") {
                        cout << "Установка .NET Runtime 7.0 ..." << endl;
                    }
                    else {
                        cout << "Installing .NET Runtime 7.0 ..." << endl;
                    }
                    system("winget install -e --id Microsoft.DotNet.Runtime.7");
                }
                else if (OS_NAME == "macOS") {
                    if (Language == "Russian") {
                        cout << "Установка .NET Runtime 7.0 ..." << endl;
                    }
                    else {
                        cout << "Installing .NET Runtime 7.0 ..." << endl;
                    }
                    system("brew install --cask dotnet");
                }
                else if (OS_NAME == "Linux") {
                    if (Language == "Russian") {
                        cout << "Установка .NET Runtime 7.0 ..." << endl;
                    }
                    else {
                        cout << "Installing .NET Runtime 7.0 ..." << endl;
                    }
                    system("snap install dotnet-runtime-70 --classic");
                    system("snap alias dotnet-runtime-70.dotnet dotnet");
                }
            }
        }
        else if (TypeInstall == "hidden") {
            if (OS_NAME == "Windows") {
                if (Language == "Russian") {
                    cout << "Установка .NET Runtime 7.0 ..." << endl;
                }
                else {
                    cout << "Installing .NET Runtime 7.0 ..." << endl;
                }
                system("winget install -e --id Microsoft.DotNet.Runtime.7");
            }
            else if (OS_NAME == "macOS") {
                if (Language == "Russian") {
                    cout << "Установка .NET Runtime 7.0 ..." << endl;
                }
                else {
                    cout << "Installing .NET Runtime 7.0 ..." << endl;
                }
                system("brew install --cask dotnet");
            }
            else if (OS_NAME == "Linux") {
                if (Language == "Russian") {
                    cout << "Установка .NET Runtime 7.0 ..." << endl;
                }
                else {
                    cout << "Installing .NET Runtime 7.0 ..." << endl;
                }
                system("snap install dotnet-runtime-70 --classic");
                system("snap alias dotnet-runtime-70.dotnet dotnet");
            }
        }
    }


    map<string, funct_t> Packages{
        { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "JetBrains WebStorm", InstallWebStorm },
        { "Docker", InstallDocker }, { "Postman", InstallPostman }, { "JetBrains RubyMine", InstallRubyMine },
        { "JetBrains Aqua", InstallAqua }, { "JetBrains CLion", InstallCLion }, { "JetBrains DataGrip", InstallDataGrip },
        { "JetBrains DataSpell", InstallDataSpell }, { "JetBrains Fleet", InstallFleet }, { "GoLand", InstallGoLand },
        { "JetBrains IntelliJ Community", InstallIntelliJCommunity }, { "JetBrains IntelliJ Ultimate", InstallIntelliJUltimate },
        { "JetBrains ReSharper", InstallReSharper }, { "JetBrains Rider", InstallRider }, { "JetBrains PhpStorm", InstallPhpStorm },
        { "JetBrains dotUltimate", InstalldotUltimate }, { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox },
        { "Postgresql", InstallPostgresql }, { "Clink", InstallCLink }, { "Ngrok", InstallNgrok }, { "Wget", InstallWget },
        { "Sublime Text", InstallSublimeText }, { "PyCharm Community", InstallPyCharmCommunity }, { "PyCharm Proffessional", InstallPyCharmProffessional },
        { "Microsoft PowerToys", InstallPowerToys }, { "Discord", InstallDiscord }, { "Telegram", InstallTelegram },
        { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer }, { "MongoDB Compass", InstallMongoDBCompass },
        { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas }, { "NodeJS", InstallNodeJS }, { "GoLang", InstallGoLang },
        { "Python 3.9", InstallPython3_9 }, { "Python 3.10", InstallPython3_10 }, { "Python 3.11", InstallPython3_11 },
        { "JDK 18", InstallJDK_18 }, { "JDK 19", InstallJDK_19 }, { "Rust", InstallRust }, { ".NET Framework", InstallNetFramework },
        { "Ruby", InstallRuby }, { "MSYS2", InstallMSYS2 }, { "Nuget", InstallNuget },
        { "GitHub Desktop", InstallGitHubDesktop }, { "GitHub CLI", InstallGitHubCLi }, { "Kubernetes", InstallKubernetes },
        { "Visual Studio Proffessional", InstallVisualStudioProffessional }, { "Visual Studio Community", InstallVisualStudioCommunity },
        { "Slack", InstallSlack }, { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome },
        { "Android Studio", InstallAndroidStudio }, { "Eclipse", InstallEclipse }, { "Kotlin", InstallKotlin },
        {".NET SDK 7.0",InstallNetSDK_7},{".NET SDK 6.0",InstallNetSDK_6},{".NET Runtime 7.0",InstallNetRuntime_7}
    };
    map<string, funct_t> PythonDevelopmentTools{
        { "Python 3.9", InstallPython3_9 }, { "Python 3.10", InstallPython3_10 }, { "Python 3.11", InstallPython3_11 },
        { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "PyCharm Community", InstallPyCharmCommunity },
        { "PyCharm Proffessional", InstallPyCharmProffessional }, { "Sublime Text", InstallSublimeText }, { "Docker", InstallDocker },
        { "Postman", InstallPostman }, { "Postgresql", InstallPostgresql }, { "MongoDB", InstallMongoDB },
        { "MongoDB Atlas", InstallMongoDBAtlas }, { "MongoDB Compass", InstallMongoDBCompass },
        { "Wget", InstallWget }, { "Discord", InstallDiscord }, { "Telegram", InstallTelegram },
        { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer }, { "GitHub Desktop", InstallGitHubDesktop },
        { "GitHub CLI", InstallGitHubCLi }, { "Kubernetes", InstallKubernetes }, { "JetBrains Fleet", InstallFleet }, { "Visual Studio Proffessional", InstallVisualStudioProffessional },
        { "Visual Studio Community", InstallVisualStudioCommunity }, { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox }, { "Slack", InstallSlack },
        { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }
    };
    map<string, funct_t> JavaScriptDevelopmentTools{
        { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "JetBrains WebStorm", InstallWebStorm },
        { "Docker", InstallDocker }, { "Postman", InstallPostman }, { "JetBrains Fleet", InstallFleet },
        { "Ngrok", InstallNgrok }, { "Wget", InstallWget }, { "Sublime Text", InstallSublimeText },
        { "Discord", InstallDiscord }, { "Telegram", InstallTelegram }, { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer },
        { "MongoDB Compass", InstallMongoDBCompass }, { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas },
        { "NodeJS", InstallNodeJS }, { "GitHub Desktop", InstallGitHubDesktop }, { "GitHub CLI", InstallGitHubCLi },
        { "Kubernetes", InstallKubernetes }, { "Visual Studio Proffessional", InstallVisualStudioProffessional },
        { "Visual Studio Community", InstallVisualStudioCommunity }, { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox },
        { "Slack", InstallSlack }, { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }
    };
    map<string, funct_t> RustDevelopmentTools{
        { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "Docker", InstallDocker },
        { "Postman", InstallPostman }, { "JetBrains Fleet", InstallFleet }, { "Wget", InstallWget },
        { "Sublime Text", InstallSublimeText }, { "Discord", InstallDiscord }, { "Telegram", InstallTelegram },
        { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer }, { "GitHub Desktop", InstallGitHubDesktop }, { "GitHub CLI", InstallGitHubCLi },
        { "Kubernetes", InstallKubernetes }, { "Rust", InstallRust }, { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas },
        { "NodeJS", InstallNodeJS }, { "MongoDB Compass", InstallMongoDBCompass }, { "Ngrok", InstallNgrok }, { "Kubernetes", InstallKubernetes },
        { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox }, { "Postgresql", InstallPostgresql },
        { "Slack", InstallSlack }, { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }
    };
    map<string, funct_t> RubyDevelopmentTools{
        { "Ruby", InstallRuby }, { "RubyMine", InstallRubyMine }, { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "Docker", InstallDocker },
        { "Postman", InstallPostman }, { "JetBrains Fleet", InstallFleet }, { "Wget", InstallWget },
        { "Sublime Text", InstallSublimeText }, { "Discord", InstallDiscord }, { "Telegram", InstallTelegram },
        { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer }, { "GitHub Desktop", InstallGitHubDesktop }, { "GitHub CLI", InstallGitHubCLi },
        { "Kubernetes", InstallKubernetes }, { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas },
        { "NodeJS", InstallNodeJS }, { "MongoDB Compass", InstallMongoDBCompass }, { "Ngrok", InstallNgrok }, { "Kubernetes", InstallKubernetes },
        { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox }, { "Postgresql", InstallPostgresql },
        { "Slack", InstallSlack }, { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }
    };
    map<string, funct_t> CppDevelopmentTools{
        { "CLion", InstallCLion }, { "Visual Studio Proffessional", InstallVisualStudioProffessional }, { "Visual Studio Community", InstallVisualStudioCommunity },
        { "JetBrains Fleet", InstallFleet }, { "Sublime Text", InstallSublimeText }, { "Ngrok", InstallNgrok },
        { "Wget", InstallWget }, { "Docker", InstallDocker }, { "Discord", InstallDiscord },
        { "Telegram", InstallTelegram }, { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer },
        { "GitHub Desktop", InstallGitHubDesktop }, { "GitHub CLI", InstallGitHubCLi }, { "JetBrains Space", InstallSpace },
        { "JetBrains ToolBox", InstallToolBox }, { "MSYS2", InstallMSYS2 }, { "Postman", InstallPostman }, { "MongoDB Compass", InstallMongoDBCompass },
        { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas }, { "Postgresql", InstallPostgresql },
        { "Slack", InstallSlack }, { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }
    };
    map<string, funct_t> CSDevelopmentTools{
        { "JetBrains Rider", InstallRider }, { ".Net Framework", InstallNetFramework }, { "Git", InstallGit }, { "VSCode", InstallVSCode },
        { "Postman", InstallPostman }, { "Nuget", InstallNuget }, { "GitHub Desktop", InstallGitHubDesktop },
        { "GitHub CLI", InstallGitHubCLi }, { "Kubernetes", InstallKubernetes }, { "Visual Studio Proffessional", InstallVisualStudioProffessional },
        { "Visual Studio Community", InstallVisualStudioCommunity }, { "JetBrains Fleet", InstallFleet }, { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox },
        { "Wget", InstallWget }, { "Sublime Text", InstallSublimeText }, { "Discord", InstallDiscord }, { "Telegram", InstallTelegram },
        { "JetBrains dotUltimate", InstalldotUltimate }, { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer }, { "MongoDB Compass", InstallMongoDBCompass },
        { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas }, { "Postgresql", InstallPostgresql },
        { "Slack", InstallSlack }, { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome },
        {".NET SDK 7.0",InstallNetSDK_7},{".NET SDK 6.0",InstallNetSDK_6},{".NET Runtime 7.0",InstallNetRuntime_7}
    };
    map<string, funct_t> CDevelopmentTools{
        { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "MSYS2", InstallMSYS2 }, { "Nuget", InstallNuget },
        { "GitHub Desktop", InstallGitHubDesktop }, { "GitHub CLI", InstallGitHubCLi }, { "Kubernetes", InstallKubernetes },
        { "Visual Studio Proffessional", InstallVisualStudioProffessional }, { "Visual Studio Community", InstallVisualStudioCommunity },
        { "Wget", InstallWget }, { "Sublime Text", InstallSublimeText }, { "JetBrains Fleet", InstallFleet },
        { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox }, { "Slack", InstallSlack },
        { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }
    };
    map<string, funct_t> GoDevelopmentTools{
        { "GoLang", InstallGoLang }, { "JetBrains Fleet", InstallFleet }, { "GoLand", InstallGoLand },
        { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "Docker", InstallDocker }, { "Postman", InstallPostman },
        { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox }, { "Postgresql", InstallPostgresql },
        { "Ngrok", InstallNgrok }, { "Wget", InstallWget }, { "Sublime Text", InstallSublimeText },
        { "Discord", InstallDiscord }, { "Telegram", InstallTelegram },
        { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer }, { "MongoDB Compass", InstallMongoDBCompass },
        { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas }, { "GitHub Desktop", InstallGitHubDesktop },
        { "GitHub CLI", InstallGitHubCLi }, { "Kubernetes", InstallKubernetes }, { "Slack", InstallSlack },
        { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }
    };
    map<string, funct_t> JavaDevelopmentTools{
        { "JDK 18", InstallJDK_18 }, { "JDK 19", InstallJDK_19 },
        { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "Docker", InstallDocker }, { "Postman", InstallPostman },
        { "JetBrains Fleet", InstallFleet }, { "JetBrains IntelliJ Community", InstallIntelliJCommunity }, { "JetBrains IntelliJ Ultimate", InstallIntelliJUltimate },
        { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox }, { "Postgresql", InstallPostgresql },
        { "Ngrok", InstallNgrok }, { "Wget", InstallWget }, { "Sublime Text", InstallSublimeText },
        { "Discord", InstallDiscord }, { "Telegram", InstallTelegram }, { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer }, { "MongoDB Compass", InstallMongoDBCompass },
        { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas },
        { "Nuget", InstallNuget }, { "Slack", InstallSlack },
        { "GitHub Desktop", InstallGitHubDesktop }, { "GitHub CLI", InstallGitHubCLi }, { "Kubernetes", InstallKubernetes },
        { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }, { "Android Studio", InstallAndroidStudio },
        { "Eclipse", InstallEclipse }, { "Kotlin", InstallKotlin }
    };
    map<string, funct_t> PhpDevelopmentTools{
        { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "Docker", InstallDocker }, { "Postman", InstallPostman },
        { "JetBrains Fleet", InstallFleet }, { "JetBrains PhpStorm", InstallPhpStorm }, { "Postgresql", InstallPostgresql },
        { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox },
        { "Ngrok", InstallNgrok }, { "Wget", InstallWget }, { "Sublime Text", InstallSublimeText },
        { "Discord", InstallDiscord }, { "Telegram", InstallTelegram },
        { "MongoDB Compass", InstallMongoDBCompass }, { "Slack", InstallSlack },
        { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas },
        { "GitHub Desktop", InstallGitHubDesktop }, { "GitHub CLI", InstallGitHubCLi }, { "Kubernetes", InstallKubernetes },
        { "Visual Studio Proffessional", InstallVisualStudioProffessional }, { "Visual Studio Community", InstallVisualStudioCommunity },
        { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }
    };
    map<string, funct_t> KotlinDevelopmentTools{
        { "Kotlin", InstallKotlin }, { "Git", InstallGit }, { "VSCode", InstallVSCode }, { "Docker", InstallDocker }, { "Postman", InstallPostman },
        { "JDK 18", InstallJDK_18 }, { "JDK 19", InstallJDK_19 }, { "JetBrains Fleet", InstallFleet }, { "JetBrains IntelliJ Community", InstallIntelliJCommunity }, { "JetBrains IntelliJ Ultimate", InstallIntelliJUltimate },
        { "JetBrains Space", InstallSpace }, { "JetBrains ToolBox", InstallToolBox }, { "Postgresql", InstallPostgresql },
        { "Ngrok", InstallNgrok }, { "Wget", InstallWget }, { "Sublime Text", InstallSublimeText },
        { "Discord", InstallDiscord }, { "Telegram", InstallTelegram }, { "VNC Server", InstallVNCServer }, { "VNC Viewer", InstallVNCViewer }, { "MongoDB Compass", InstallMongoDBCompass },
        { "MongoDB", InstallMongoDB }, { "MongoDB Atlas", InstallMongoDBAtlas },
        { "Nuget", InstallNuget }, { "Slack", InstallSlack },
        { "GitHub Desktop", InstallGitHubDesktop }, { "GitHub CLI", InstallGitHubCLi }, { "Kubernetes", InstallKubernetes },
        { "Vim", InstallVim }, { "NeoVim", InstallNeoVim }, { "Google Chrome", InstallGoogleChrome }, { "Android Studio", InstallAndroidStudio },
        { "Eclipse", InstallEclipse }
    };

    void PythonDevelopment()
    {
        for (const auto& element : PythonDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void JavaScriptDevelopment()
    {
        for (const auto& element : JavaScriptDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void RustDevelopment()
    {
        for (const auto& element : RustDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void RubyDevelopment()
    {
        for (const auto& element : RubyDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void CppDevelopment()
    {
        for (const auto& element : CppDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void CSDevelopment()
    {
        for (const auto& element : CSDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void CDevelopment()
    {
        for (const auto& element : CDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void GoDevelopment()
    {
        for (const auto& element : GoDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void JavaDevelopment()
    {
        for (const auto& element : JavaDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void PHPDevelopment()
    {
        for (const auto& element : PhpDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    void KotlinDevelopment()
    {
        for (const auto& element : KotlinDevelopmentTools) {
            string name = element.first;
            cout << name << endl;
            element.second();
        }
    }
    map<int, funct_t> DevelopmentPacks{
        {1,AppInstaller::PythonDevelopment},{2,AppInstaller::JavaDevelopment},
        {3,AppInstaller::CppDevelopment},{4,AppInstaller::JavaDevelopment},
        {5,AppInstaller::GoDevelopment},{6,AppInstaller::RustDevelopment},
        {7,AppInstaller::RubyDevelopment},{8,AppInstaller::CDevelopment},
        {9,AppInstaller::CSDevelopment},{10,AppInstaller::PHPDevelopment},
        {11,AppInstaller::KotlinDevelopment}
    };
}