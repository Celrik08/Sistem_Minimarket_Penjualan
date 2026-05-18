void tambah_barang(Barang *barang, int &jumlah_barang){

    try{
        cout<<"Silahkan Tambah Barang disini\n";

        Barang *ptr = barang + jumlah_barang;

        lihat_barang(barang, jumlah_barang);

        while(true){
            cout<<"Id Barang : ";
            getline(cin, ptr->id_barang);

                bool Huruf = false;

            for(char c : ptr->id_barang){
                if(isalpha(c)){
                    Huruf = true;
                    break;
                }
            }

            if(ptr->id_barang <= "0" || 
                ptr->id_barang.find(' ') != string::npos){

                cout<<"Error: Id barang tidak boleh kosong, mengandung spasi, atau negatif!"<<endl;
                cout << "Tekan Enter untuk kembali ke menu...";
                cin.get();
                return;
            }
            else if(Huruf){

                cout<<"Error: Id barang tidak boleh menggunakan huruf!"<<endl;
                cout << "Tekan Enter untuk kembali ke menu...";
                cin.get();
                return;
            }
            else{

                for(int i = 0; i < jumlah_barang; i++){
                    if((barang+i)->id_barang == ptr->id_barang){
                        cout<<"Error: ID barang sudah digunakan!"<<endl;
                        cout << "Tekan Enter untuk kembali ke menu...";
                        cin.get();
                        return;
                    }
                }
                break;
            }
        }

        while(true){
            cout<<"Nama Barang : ";
            getline(cin, ptr->nama_barang);

            if(ptr->nama_barang.empty()){
                cout << "Error: Nama barang tidak boleh kosong!" << endl;
            } else break;
        }

        while(true){
            cout<<"Jenis Barang : ";
            getline(cin, ptr->jenis_barang.kategori);

            if(ptr->jenis_barang.kategori.empty()){
                cout << "Error: Jenis barang tidak boleh kosong!" << endl;
            } else break;
        }

        while(true){
            cout << "Harga Rp: ";

            string input_harga;
            getline(cin, input_harga);

            if(input_harga.empty()){
                cout << "Error: Harga tidak boleh kosong!" << endl;
                cout << "Tekan Enter untuk kembali ke menu...";
                cin.get();
                return;
            }

            if(input_harga.find('.') != string::npos || 
                input_harga.find(',') != string::npos){

                cout << "Error: Inputan tidak boleh berisi titik atau koma!\n";
                continue;
            }

            try{
                ptr->harga_barang = stoi(input_harga);

                if(ptr->harga_barang <= 0){
                    cout << "Error: Harga tidak boleh 0 atau minus!\n";
                    continue;
                }
                break;
            }
            catch(...){
                cout << "Error: Harga harus berupa angka!\n";
            }
        }

        while(true){

            cout << "Stok : ";

            string input_stok;
            getline(cin, input_stok);

            if(input_stok.empty()){
                cout << "Error: Stok tidak boleh kosong!" << endl;
                cout << "Tekan Enter untuk kembali ke menu...";
                cin.get();
                return;
            }

            if(input_stok.find('.') != string::npos || 
                input_stok.find(',') != string::npos){

                cout << "Error: Inputan tidak boleh berisi titik atau koma!\n";
                continue;
            }

            try{
                ptr->stok = stoi(input_stok);

                if(ptr->stok <= 0){
                    cout << "Error: Stok tidak boleh 0 atau minus!\n";
                    continue;
                }
                break;
            }
            catch(...){
                cout << "Error: Stok harus berupa angka!\n";
            }
        }

        Barang_json(*ptr);

        jumlah_barang++;

        Output("Barang berhasil ditambahkan. Silahkan cek datanya");
        cout << "Tekan Enter untuk kembali ke menu...";
        cin.get();
    }
    catch(exception &e){
        cout<<"Error: "<<e.what()<<endl;
    
    }
}

void Ubah_barang_json(Barang *barang, int jumlah_barang){
    ofstream file("Database/barang.json");

    if(file.is_open()){

        file << "[\n";

        for(int j = 0; j < jumlah_barang; j++){

            file << "   {\n";
            file << "        \"id_barang\": \"" << (barang+j)->id_barang << "\",\n";
            file << "        \"nama_barang\": \"" << (barang+j)->nama_barang << "\",\n";
            file << "        \"kategori\": \"" << (barang+j)->jenis_barang.kategori << "\",\n";
            file << "        \"harga\": " << (barang+j)->harga_barang << ",\n";
            file << "        \"stok\": " << (barang+j)->stok << "\n";
            file << "   }";

            if(j != jumlah_barang - 1){
                file << ",";
            }

            file << "\n";
        }

        file << "]\n";

        file.close();
    }
}

void ubah_barang(Barang *barang, int &jumlah_barang){

    try{

        lihat_barang(barang, jumlah_barang);

        string id;
        cout<<"Masukkan ID barang yang ingin anda ubah : ";
        getline(cin, id);

        if(id.empty() || id.find_first_not_of(' ') == string::npos || id.find_first_not_of("0123456789") != string::npos){

            cout << "Error: ID tidak boleh kosong, hanya spasi, atau mengandung huruf!" << endl;
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }

        bool ditemukan = false;

        for(int i=0;i<jumlah_barang;i++){

            if((barang+i)->id_barang == id){

                ditemukan = true;

                string nama_baru, jenis_baru;
                int harga_baru, stok_baru;

                cout<<"Nama Barang (kosongkan jika tidak ingin anda mengubah nya) : ";
                getline(cin,nama_baru);

                if(nama_baru != ""){
                    (barang+i)->nama_barang = nama_baru;
                }

                cout<<"Jenis Barang (kosongkan jika tidak ingin anda mengubah nya) : ";
                getline(cin,jenis_baru);

                if(jenis_baru != ""){
                    (barang+i)->jenis_barang.kategori = jenis_baru;
                }

                while(true){

                    cout<<"Harga (Kosongkan jika tidak ingin anda mengubah nya) Rp: ";

                    string input_harga;
                    getline(cin, input_harga);

                    if(input_harga.empty()){
                        harga_baru = 0;
                        break;
                    }

                    stringstream ss(input_harga);
                    ss >> harga_baru;

                    if(ss.fail()){
                        cout<<"Error: Harga harus berupa angka!\n";
                        continue;
                    }

                    if(harga_baru < 0 || input_harga.find(' ') != string::npos){
                        cout << "Error: Harga tidak boleh negatif atau mengandung spasi!" << endl;
                        continue;
                    }
                    if(input_harga.find('.') != string::npos || 
                        input_harga.find(',') != string::npos){

                        cout << "Error: Inputan tidak boleh berisi titik atau koma!\n";
                        continue;
                    }

                    break;
                }

                if(harga_baru != 0){
                    (barang+i)->harga_barang = harga_baru;
                }

                while(true){

                    cout<<"Stok Baru (Kosongkan jika tidak ingin anda mengubah nya) : ";

                    string input_stok;
                    getline(cin, input_stok);

                    if(input_stok.empty()){
                        stok_baru = 0;
                        break;
                    }

                    stringstream ss(input_stok);
                    ss >> stok_baru;

                    if(ss.fail()){
                        cout<<"Error: Stok harus berupa angka!\n";
                        continue;
                    }

                    if(stok_baru < 0){
                        cout << "Error: Stok tidak boleh negatif!" << endl;
                        continue;
                    }

                    break;
                }

                if(stok_baru != 0){
                    (barang+i)->stok = stok_baru;
                }

                Ubah_barang_json(barang, jumlah_barang);

                Output("Data berhasil diubah, silahkan cek datannya");
                cout << "Tekan Enter untuk kembali ke menu...";
                cin.get();

                return;
            }
        }

        if(!ditemukan){
            cout << "Error: ID barang tidak ditemukan!, silahkan ubah data dengan benar" << endl;
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }
    }
    catch(exception &e){
        cout<<"Error: "<<e.what()<<endl;
    }
}

void hapus_barang(Barang *barang, int *jumlah_barang){

    try{

        lihat_barang(barang, *jumlah_barang);

        string id;
        cout<<"Masukkan ID barang yang ingin anda hapus: ";
        getline(cin, id);

        if(id.empty() || id.find_first_not_of(' ') == string::npos || id.find_first_not_of("0123456789") != string::npos){

            cout << "ID tidak boleh kosong, hanya spasi, atau mengandung huruf!" << endl;
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }

        bool ditemukan = false;

        for(int i=0;i<*jumlah_barang;i++){

            if((barang+i)->id_barang == id){

                ditemukan = true;

                char konfirmasi;

                while(true){

                    cout << "Apakah anda yakin ingin menghapus barang ini? (y/n): ";
                    cin >> konfirmasi;

                    if(konfirmasi == 'n' || konfirmasi == 'N'){

                        cin.ignore();

                        cout << "Penghapusan barang dibatalkan!" << endl;
                        cout << "Tekan Enter untuk kembali ke menu...";
                        cin.get();

                        return;
                    }

                    else if(konfirmasi == 'y' || konfirmasi == 'Y'){

                        break;
                    }

                    else{

                        cout << "Input konfirmasi tidak valid!" << endl;
                    }
                }

                for(int j=i;j<*jumlah_barang-1;j++){
                    *(barang+j) = *(barang+j+1);
                }

                (*jumlah_barang)--;

                ofstream file("Database/barang.json");

                if(file.is_open()){
                    file << "[\n";
                    for(int i = 0; i < *jumlah_barang; i++){
                        file << "   {\n";
                        file << "        \"id_barang\": \"" << (barang+i)->id_barang << "\",\n";
                        file << "        \"nama_barang\": \"" << (barang+i)->nama_barang << "\",\n";
                        file << "        \"kategori\": \"" << (barang+i)->jenis_barang.kategori << "\",\n";
                        file << "        \"harga\": " << (barang+i)->harga_barang << ",\n";
                        file << "        \"stok\": " << (barang+i)->stok << "\n";
                        file << "   }";
                        if(i != *jumlah_barang - 1) file << ",";
                        file << "\n";
                    }
                    file << "]\n";
                    file.close();
                }

                cin.ignore();
                Output("Barang berhasil dihapus. Silahkan cek datanya");
                cout << "Tekan Enter untuk kembali ke menu...";
                cin.get();
                return;
            }
        }

        if(!ditemukan){
            cout << "Error: ID barang tidak ditemukan!, silahkan hapus data dengan benar" << endl;
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }
    }
    catch(exception &e){
        cout<<"Error: "<<e.what()<<endl;
    }
}

void sort_id(Barang *barang, int jumlah_barang){
    for(int i=0;i<jumlah_barang-1;i++){
        for(int j=0;j<jumlah_barang-i-1;j++){
            if((barang+j)->id_barang > (barang+j+1)->id_barang){
                Barang temp = *(barang+j);
                *(barang+j) = *(barang+j+1);
                *(barang+j+1) = temp;
            }
        }
    }

    ofstream file("Database/barang.json");

    if(file.is_open()){
        file << "[\n";
        for(int i = 0; i < jumlah_barang; i++){
            file << "   {\n";
            file << "        \"id_barang\": \"" << (barang+i)->id_barang << "\",\n";
            file << "        \"nama_barang\": \"" << (barang+i)->nama_barang << "\",\n";
            file << "        \"kategori\": \"" << (barang+i)->jenis_barang.kategori << "\",\n";
            file << "        \"harga\": " << (barang+i)->harga_barang << ",\n";
            file << "        \"stok\": " << (barang+i)->stok << "\n";
            file << "   }";

            if(i != jumlah_barang - 1) file << ",";
            file << "\n";
        }
        file << "]\n";
        file.close();
    }

    Output("Barang berhasil terurut (ascending). Silahkan lihat datanya");
    return;
}

int search_nama(Barang *barang, int jumlah_barang, string nama){
    for(int i = 0; i < jumlah_barang; i++){
        if((barang+i)->nama_barang == nama){
            return i;
        }
    }
    return -1;
}

void lihat_laporan_transaksi(){

    ifstream file("Database/laporan_transaksi.json");

    if(file.is_open()){

        string baris;

        cout << "\nDATA LAPORAN TRANSAKSI\n";
        cout << "-------------------------------------------------------------\n";
        cout << left << setw(15) << "ID Laporan"
             << setw(15) << "ID Transaksi"
             << setw(15) << "Tanggal"
             << setw(15) << "Kasir"
             << setw(15) << "Pembeli"
             << setw(15) << "Total (Rp)" << endl;
        cout << "-------------------------------------------------------------\n";

        string id_laporan, id_transaksi, tanggal, kasir, pembeli;
        int total;

        while(getline(file, baris)){

            if(baris.find("\"id_laporan\"") != string::npos){
                int pos = baris.find(":") + 1;
                id_laporan = baris.substr(pos);
                id_laporan.erase(remove(id_laporan.begin(), id_laporan.end(), ','), id_laporan.end());
            }

            if(baris.find("\"id_transaksi\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                id_transaksi = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"tanggal_transaksi\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                tanggal = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"nama_kasir\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                kasir = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"nama_pembeli\"") != string::npos){
                int pos1 = baris.find("\"", baris.find(":")) + 1;
                int pos2 = baris.find("\"", pos1);
                pembeli = baris.substr(pos1, pos2 - pos1);
            }

            if(baris.find("\"total_belanja\"") != string::npos){
                int pos = baris.find(":") + 1;
                total = stoi(baris.substr(pos));

                cout << left << setw(15) << id_laporan
                     << setw(15) << id_transaksi
                     << setw(15) << tanggal
                     << setw(15) << kasir
                     << setw(15) << pembeli
                     << setw(15) << format_harga(total) << endl;
            }
        }

        file.close();
        cout << "\nTekan Enter untuk kembali...";
        cin.get();

    } else {
        cout << "Gagal membuka file laporan_transaksi.json\n";
    }
}

void register_admin(User *users, int &jumlah_user){

    cout<<"\nMENU REGISTER ADMIN\n";

    users[jumlah_user].id_user = jumlah_user + 1;
    users[jumlah_user].role = "kasir";

    cout<<"Username : ";

    while(true){

        getline(cin, users[jumlah_user].username);

        if(users[jumlah_user].username.empty()){
            cout << "Error: Username tidak boleh kosong!" << endl;
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }

        bool username_sama = false;

        for(int i = 0; i < jumlah_user; i++){

            if(users[i].username == users[jumlah_user].username){
                username_sama = true;
                break;
            }
        }

        if(username_sama){
            cout << "Error: Username sudah digunakan, silahkan input username lain!\n";
            cout << "Username : ";
        }
        else{
            break;
        }
    }

    cout<<"Password : ";
    while(true){

        getline(cin, users[jumlah_user].password);

        if(users[jumlah_user].password.empty()){
            cout << "Error: Password tidak boleh kosong!" << endl;
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }

        break;
    }

    cout << "Role terpilih : " << users[jumlah_user].role << endl;

    User_json(users[jumlah_user]);

    jumlah_user++;

    cout << "Tekan Enter untuk kembali ke menu...";
    cin.get();
}

void menu_admin(Barang *barang, int &jumlah_barang){
    
    load_barang(barang, jumlah_barang);

    load_user(users, jumlah_user);

    system("cls");
    int posisi;

    while(true){
        vector<string> daftarMenu = {
            "Menu Tambah Barang",
            "Menu Lihat Barang",
            "Menu Ubah Barang",
            "Menu Hapus Barang",
            "Sorting Id (Bubble Sort)",
            "Searching Nama (Linear Search)",
            "Laporan Transaksi",
            "Buat Akun (Kasir)",
            "Keluar"
        };

        posisi = pilihMenu(daftarMenu, "SELEMAT DATANG DI MINIMARKET MUHAMMAD FAJAR") + 1;

        if(posisi == 1){
            tambah_barang(barang,jumlah_barang);
        }
        else if(posisi == 2){
            lihat_barang(barang,jumlah_barang);
            cout << "\nTekan Enter untuk kembali...";
            cin.get();
        }
        else if(posisi == 3){
            ubah_barang(barang,jumlah_barang);
        }
        else if(posisi == 4){
            hapus_barang(barang,&jumlah_barang);
        }
        else if(posisi == 5){
            sort_id(barang,jumlah_barang);
            lihat_barang(barang,jumlah_barang);
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
        }
        else if(posisi == 6){
            
            lihat_barang(barang,jumlah_barang);

            string nama;
            cout<<"Masukkan nama barang: ";
            getline(cin, nama);

            int hasil = search_nama(barang, jumlah_barang, nama);

            if(hasil != -1){
                cout<<"Data ditemukan:\n";
                cout<<"ID: "<<(barang+hasil)->id_barang<<endl;
                cout<<"Nama: "<<(barang+hasil)->nama_barang<<endl;
                cout<<"Kategori: "<<(barang+hasil)->jenis_barang.kategori<<endl;
                cout<<"Harga: "<<(barang+hasil)->harga_barang<<endl;
                cout<<"Stok: "<<(barang+hasil)->stok<<endl;

                cout << "Tekan Enter untuk kembali ke menu...";
                cin.get();
            } else {
                cout<<"Data tidak ditemukan\n";
                cout << "Tekan Enter untuk kembali ke menu...";
                cin.get();
            }
        }
        else if(posisi == 7){
            lihat_laporan_transaksi();
        }
        else if(posisi == 8){
            register_admin(users, jumlah_user);
        }
        else if(posisi == 9){
            cout<<"Anda Keluar dari MENU MINIMARKET MUHAMMAD FAJAR\n";
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            break;
        }
    }
}