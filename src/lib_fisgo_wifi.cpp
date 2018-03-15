#include "lib_fisgo_wifi.h"


Fisgo_Wifi::Fisgo_Wifi()
{
    Fisgo_Wifi_DB::instance();
}

Fisgo_Wifi::~Fisgo_Wifi()
{
    Fisgo_Wifi_DB::releaseInstance();
}

string Fisgo_Wifi::convert_cyrillic(string ssid)
{
    string tmp = "";
    for ( uint32_t index = 0; index < ssid.size(); )
    {
        uint64_t code = 0;
        memcpy(&code, &ssid[index], sizeof(code));

        uint64_t ibm1 = code & CYR_TEST_1;
        uint64_t ibm2 = code & CYR_TEST_2;

        if ( (ibm1 == CYR_TEST_1) || (ibm2 == CYR_TEST_2) )
        {
            switch ( code )
            {
            case CYR_UP_1 : tmp.append("�"); break;
            case CYR_UP_2 : tmp.append("�"); break;
            case CYR_UP_3 : tmp.append("�"); break;
            case CYR_UP_4 : tmp.append("�"); break;
            case CYR_UP_5 : tmp.append("�"); break;
            case CYR_UP_6 : tmp.append("�"); break;
            case CYR_UP_7 : tmp.append("�"); break;
            case CYR_UP_8 : tmp.append("�"); break;
            case CYR_UP_9 : tmp.append("�"); break;
            case CYR_UP_10: tmp.append("�"); break;
            case CYR_UP_11: tmp.append("�"); break;
            case CYR_UP_12: tmp.append("�"); break;
            case CYR_UP_13: tmp.append("�"); break;
            case CYR_UP_14: tmp.append("�"); break;
            case CYR_UP_15: tmp.append("�"); break;
            case CYR_UP_16: tmp.append("�"); break;
            case CYR_UP_17: tmp.append("�"); break;
            case CYR_UP_18: tmp.append("�"); break;
            case CYR_UP_19: tmp.append("�"); break;
            case CYR_UP_20: tmp.append("�"); break;
            case CYR_UP_21: tmp.append("�"); break;
            case CYR_UP_22: tmp.append("�"); break;
            case CYR_UP_23: tmp.append("�"); break;
            case CYR_UP_24: tmp.append("�"); break;
            case CYR_UP_25: tmp.append("�"); break;
            case CYR_UP_26: tmp.append("�"); break;
            case CYR_UP_27: tmp.append("�"); break;
            case CYR_UP_28: tmp.append("�"); break;
            case CYR_UP_29: tmp.append("�"); break;
            case CYR_UP_30: tmp.append("�"); break;
            case CYR_UP_31: tmp.append("�"); break;
            case CYR_UP_32: tmp.append("�"); break;
            case CYR_UP_33: tmp.append("�"); break;

            case CYR_LOW_1 : tmp.append("�"); break;
            case CYR_LOW_2 : tmp.append("�"); break;
            case CYR_LOW_3 : tmp.append("�"); break;
            case CYR_LOW_4 : tmp.append("�"); break;
            case CYR_LOW_5 : tmp.append("�"); break;
            case CYR_LOW_6 : tmp.append("�"); break;
            case CYR_LOW_7 : tmp.append("�"); break;
            case CYR_LOW_8 : tmp.append("�"); break;
            case CYR_LOW_9 : tmp.append("�"); break;
            case CYR_LOW_10: tmp.append("�"); break;
            case CYR_LOW_11: tmp.append("�"); break;
            case CYR_LOW_12: tmp.append("�"); break;
            case CYR_LOW_13: tmp.append("�"); break;
            case CYR_LOW_14: tmp.append("�"); break;
            case CYR_LOW_15: tmp.append("�"); break;
            case CYR_LOW_16: tmp.append("�"); break;
            case CYR_LOW_17: tmp.append("�"); break;
            case CYR_LOW_18: tmp.append("�"); break;
            case CYR_LOW_19: tmp.append("�"); break;
            case CYR_LOW_20: tmp.append("�"); break;
            case CYR_LOW_21: tmp.append("�"); break;
            case CYR_LOW_22: tmp.append("�"); break;
            case CYR_LOW_23: tmp.append("�"); break;
            case CYR_LOW_24: tmp.append("�"); break;
            case CYR_LOW_25: tmp.append("�"); break;
            case CYR_LOW_26: tmp.append("�"); break;
            case CYR_LOW_27: tmp.append("�"); break;
            case CYR_LOW_28: tmp.append("�"); break;
            case CYR_LOW_29: tmp.append("�"); break;
            case CYR_LOW_30: tmp.append("�"); break;
            case CYR_LOW_31: tmp.append("�"); break;
            case CYR_LOW_32: tmp.append("�"); break;
            case CYR_LOW_33: tmp.append("�"); break;
            default: break;
            }

            index += CYR_SIZE;
        }
        else
        {
            tmp.push_back( ssid.at(index) );
            ++index;
        }
    }

    return tmp;
}

bool Fisgo_Wifi::scan_net()
{
    system("wpa_cli -i wlan0 scan");
    usleep(10000);

    system("wpa_cli -i wlan0 scan_results > /FisGo/wifi/wifi_scan_results");
    usleep(1000);

    ifstream file;
    file.open( "/FisGo/wifi/wifi_scan_results" );

    if ( file.is_open() )
    {
        string str;
        while ( getline( file,  str) )
        {
            Wifi_Data data;
            vector<string> list = splitString(str, '\t');

            if ( list.size() < 5 )
            {
                continue;
            }

            data.bssid = list.at(0);
            data.freq  = stoi( list.at(1), nullptr, 10);
            data.level = stoi( list.at(2), nullptr, 10);
            data.flags = list.at(3);
            data.ssid  = list.at(4);

            // ��ॢ�� ����� �� � �⠥�� ��� �� ����稨 ��ਫ���᪨� ᨬ�����
            data.ssid  = convert_cyrillic( data.ssid );

            if ( data.ssid.compare("") == 0 )
            {
                continue;
            }

            bool skip = false;
            for ( Wifi_Data wifiName : wifi_networks )
            {
                if ( wifiName.ssid.compare( data.ssid ) == 0 )
                {
                    skip = true;
                    break;
                }
            }

            if ( skip )
            {
                continue;
            }

            if ( data.flags.find("WPA-PSK") != string::npos )
            {
                data.sec = WIFI_SEC_WPA;
            }
            else if ( data.flags.find("WPA2-PSK") != string::npos )
            {
                data.sec = WIFI_SEC_WPA2;
            }
            else if ( data.flags.find("WEP") != string::npos )
            {
                data.sec = WIFI_SEC_WEP;
            }
            else if ( data.flags.find("EAP") != string::npos )
            {
                data.sec = WIFI_SEC_EAP;
            }
            else
            {
                continue;
            }

            wifi_networks.push_back( data );
        }

        file.close();
        remove("/FisGo/wifi/wifi_scan_results");
    }
    else
    {
        return false;
    }

    return true;
}

Fisgo_Wifi::WIFI_STATUS Fisgo_Wifi::get_status()
{
    return state;
}

bool Fisgo_Wifi::create_cfg(const Wifi_Data &data, string password)
{
    ofstream wpa_conf;
    wpa_conf.open("/etc/wpa_supplicant.conf", ofstream::trunc);

    if ( wpa_conf.is_open() )
    {
        wpa_conf << "ctrl_interface=/var/run/wpa_supplicant" << endl;
        wpa_conf << "network={" << endl;
        wpa_conf << "bssid=" << data.bssid << ""<< endl;
        wpa_conf << "psk=\"" << password << "\""<< endl;
        wpa_conf << "}" << endl;
        wpa_conf.flush();
        wpa_conf.close();

        sync();
        sync();

        return true;
    }

    return false;
}

bool Fisgo_Wifi::create_def_cfg()
{
    ofstream wpa_conf;
    wpa_conf.open("/etc/wpa_supplicant.conf", ofstream::trunc);

    if ( wpa_conf.is_open() )
    {
        wpa_conf << "ctrl_interface=/var/run/wpa_supplicant" << endl;
        wpa_conf << "network={" << endl;
        wpa_conf << "bssid=BE:EF:13:00:00:00"<< endl;
        wpa_conf << "psk=\"12345678\""<< endl;
        wpa_conf << "}" << endl;
        wpa_conf.flush();
        wpa_conf.close();

        sync();
        sync();

        return true;
    }

    return false;
}

int8_t Fisgo_Wifi::is_wpa_run()
{
    int8_t err = -1;

    for ( int8_t ret = 5; ret > 0; --ret )
    {
        system("pidof wpa_supplicant > /FisGo/wifi/wpa_supp_pid");

        usleep(1000);

        ifstream pid("/FisGo/wifi/wpa_supp_pid");

        if ( pid.is_open() )
        {
            string pid_str;
            getline( pid, pid_str );
            pid.close();
            remove("/FisGo/wifi/wpa_supp_pid");

            if ( !pid_str.empty() )
            {
                err = 0;
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            continue;
        }
    }

    return err;
}

bool Fisgo_Wifi::savePassword(string bssid, string password)
{
    return Fisgo_Wifi_DB::instance()->savePassword( bssid, password );
}

string Fisgo_Wifi::getPassword(string bssid)
{
    return Fisgo_Wifi_DB::instance()->getPassword( bssid );
}

bool Fisgo_Wifi::init()
{
    turn_on();

    int8_t err = -1;

    for ( uint8_t try_count = 0; try_count < 2; ++try_count )
    {

#ifdef DREAMKAS_F
        system("wpa_supplicant -Dwext -iwlan0 -c/etc/wpa_supplicant.conf &");
#else
        system("wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wpa_supplicant.conf &");
#endif

        usleep(10000);

        err = is_wpa_run();

        if ( err == -1 )
        {
            create_def_cfg();
        }
        else
        {
            break;
        }
    }

    // ����⪠ ����祭�� ���� � ��᫥���� ������祭��� ��
    // ����⪠ ����祭�� IP ����
    // 5 ����⮪ ����祭�� � ���ࢠ��� � 3 ᥪ㭤�
    system("udhcpc -n -t 5 -i wlan0");

    // ���������� ⥪�饣� ����� wi-fi
    status();

    if ( err == -1 )
    {
        return false;
    }

    return true;
}

void Fisgo_Wifi::deinit()
{
    system("pidof wpa_supplicant > /FisGo/wifi/wpa_supp_pid");

    usleep(10000);

    ifstream pid("/FisGo/wifi/wpa_supp_pid");

    if ( pid.is_open() )
    {
        string pid_str;
        getline( pid, pid_str );
        pid.close();

        if ( !pid_str.empty() )
        {
            system("killall wpa_supplicant");
            usleep(10000);
        }
    }

    remove("/FisGo/wifi/wpa_supp_pid");
}

bool Fisgo_Wifi::clear()
{
    return Fisgo_Wifi_DB::instance()->clearDB();
}

Fisgo_Wifi::WIFI_STATUS Fisgo_Wifi::status()
{
    system("wpa_cli status | grep wpa_state | awk -F \"=\" '{print $2}' > /FisGo/wifi/wpa_state");

    usleep(10000);

    ifstream wpa_state("/FisGo/wifi/wpa_state");

    if ( wpa_state.is_open() )
    {
        string wpa_state_str;
        getline( wpa_state, wpa_state_str );
        wpa_state.close();
        remove("/FisGo/wifi/wpa_state");

        if ( wpa_state_str.compare("INTERFACE_DISABLED") == 0 )
        {
            state = WIFI_INTERFACE_DISABLED;
        }
        else if ( wpa_state_str.compare("SCANNING") == 0 )
        {
            state = WIFI_SCANNING;
        }
        else if ( wpa_state_str.compare("ASSOCIATING") == 0 )
        {
            state = WIFI_ASSOCIATING;
        }
        else if ( wpa_state_str.compare("COMPLETED") == 0 )
        {
            state = WIFI_COMPLETED;
        }
        else if ( wpa_state_str.compare("DISCONNECTED") == 0 )
        {
            state = WIFI_DISCONNECTED;
        }
        else
        {
            state = WIFI_UNKNOWN_STATE;
        }
    }
    else
    {
        state = WIFI_UNKNOWN_STATE;
    }

    return state;
}

bool Fisgo_Wifi::is_ip_available()
{
    system("wpa_cli status | grep ip_address | awk -F \"=\" '{print $2}' > /FisGo/wifi/wpa_ip_available");

    usleep(10000);

    ifstream ip("/FisGo/wifi/wpa_ip_available");

    if ( ip.is_open() )
    {
        string available_ip;
        getline( ip, available_ip );
        ip.close();
        remove("/FisGo/wifi/wpa_ip_available");

        if ( !available_ip.empty() )
        {
            return true;
        }
    }

    return false;
}

bool Fisgo_Wifi::connect(uint8_t idNet, string password)
{
    if ( is_wpa_run() == 0 )
    {
        deinit();
    }

    if ( wifi_networks.size() < idNet )
    {
        return false;
    }

    if ( create_cfg( wifi_networks.at(idNet), password ) == false )
    {
        return false;
    }

    if ( init() == false )
    {
        return false;
    }

    // ����⪠ ����祭�� IP ����
    // 5 ����⮪ ����祭�� � ���ࢠ��� � 3 ᥪ㭤�
    system("udhcpc -n -t 5 -i wlan0");

    if ( is_ip_available() == false )
    {
        return false;
    }

    // ���������� ⥪�饣� ����� wi-fi
    status();

    savePassword( wifi_networks.at(idNet).bssid, password );

    return true;
}

void Fisgo_Wifi::turn_on()
{
    system("ifconfig wlan0 up");
    sleep(3);
}

void Fisgo_Wifi::turn_off()
{
    system("ifconfig wlan0 down");
    sleep(3);
}

vector<Fisgo_Wifi::Wifi_Data> Fisgo_Wifi::getWifi_networks() const
{
    return wifi_networks;
}

vector<string> Fisgo_Wifi::splitString(string divisibleStr, char delimiter)
{
    vector<string> rv;

    if ( divisibleStr.empty() )
        return rv;

    string tmpString;
    istringstream streamStr(divisibleStr);

    while ( getline(streamStr, tmpString, delimiter) )
    {
       rv.push_back( tmpString );
    }

    return rv;
}
