void simpan_detail_transaksi_json(Detail_Transaksi detail){

    load_detail_transaksi(detail_transaksi, jumlah_detail_transaksi);

    bool barang_sama = false;

    for(int i = 0; i < jumlah_detail_transaksi; i++){

        if(detail_transaksi[i].id_barang == detail.id_barang){

            detail_transaksi[i].jumlah_beli += detail.jumlah_beli;

            detail_transaksi[i].subtotal += detail.subtotal;

            barang_sama = true;

            break;
        }
    }

    if(!barang_sama){

        detail.id_detail_transaksi = jumlah_detail_transaksi + 1;

        detail_transaksi[jumlah_detail_transaksi] = detail;

        jumlah_detail_transaksi++;
    }

    ofstream file("Database/detail_transaksi.json");

    if(file.is_open()){

        file << "[\n";

        for(int i = 0; i < jumlah_detail_transaksi; i++){

            file << "   {\n";

            file << "        \"id_detail_transaksi\": "<< detail_transaksi[i].id_detail_transaksi << ",\n";
            file << "        \"id_transaksi\": \""<< detail_transaksi[i].id_transaksi << "\",\n";
            file << "        \"id_barang\": \""<< detail_transaksi[i].id_barang << "\",\n";
            file << "        \"nama_barang\": \""<< detail_transaksi[i].nama_barang << "\",\n";
            file << "        \"harga_barang\": "<< detail_transaksi[i].harga_barang << ",\n";
            file << "        \"jumlah_beli\": "<< detail_transaksi[i].jumlah_beli << ",\n";
            file << "        \"subtotal\": "<< detail_transaksi[i].subtotal << "\n";

            file << "   }";

            if(i != jumlah_detail_transaksi - 1){
                file << ",";
            }

            file << "\n";
        }

        file << "]\n";

        file.close();
    }
}

void update_barang_json(Barang *barang, int jumlah_barang){

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

            if(i != jumlah_barang - 1){
                file << ",";
            }

            file << "\n";
        }

        file << "]\n";

        file.close();
    }
}

string generate_id_transaksi(){

    load_laporan_transaksi_kasir(laporan_kasir, jumlah_laporan_kasir);

    time_t now = time(0);
    tm *ltm = localtime(&now);

    int hari = ltm->tm_mday;
    int bulan = 1 + ltm->tm_mon;
    int tahun = 1900 + ltm->tm_year;

    string tanggal =
        (hari < 10 ? "0" : "") + to_string(hari) +
        (bulan < 10 ? "0" : "") + to_string(bulan) +
        to_string(tahun);

    int nomor_terakhir = 0;

    for(int i = 0; i < jumlah_laporan_kasir; i++){

        string id = laporan_kasir[i].id_transaksi;

        string tanggal_file = id.substr(3, 8);

        if(tanggal_file == tanggal){

            string nomor = id.substr(11);

            nomor_terakhir = max(
                nomor_terakhir,
                stoi(nomor)
            );
        }
    }

    nomor_terakhir++;

    string nomor_baru;

    if(nomor_terakhir < 10){
        nomor_baru = "00" + to_string(nomor_terakhir);
    }
    else if(nomor_terakhir < 100){
        nomor_baru = "0" + to_string(nomor_terakhir);
    }
    else{
        nomor_baru = to_string(nomor_terakhir);
    }

    return "TRX" + tanggal + nomor_baru;
}

void simpan_laporan_transaksi_kasir(string id_transaksi, string nama_pembeli, int total_belanja){

    load_detail_transaksi(detail_transaksi, jumlah_detail_transaksi);

    load_laporan_transaksi_kasir(laporan_kasir, jumlah_laporan_kasir);

    ifstream infile("Database/laporan_transaksi_kasir.json");

    string isi_lama = "";
    string baris;

    if(infile.is_open()){

        while(getline(infile, baris)){
            isi_lama += baris + "\n";
        }

        infile.close();
    }

    string cek_isi = isi_lama;

    cek_isi.erase(remove(cek_isi.begin(), cek_isi.end(), ' '), cek_isi.end());
    cek_isi.erase(remove(cek_isi.begin(), cek_isi.end(), '\n'), cek_isi.end());

    ofstream file("Database/laporan_transaksi_kasir.json");

    time_t now = time(0);
    tm *ltm = localtime(&now);

    int hari = ltm->tm_mday;
    int bulan = 1 + ltm->tm_mon;
    int tahun = 1900 + ltm->tm_year;

    if(file.is_open()){

        if(isi_lama.empty() || cek_isi == "[]"){

            file << "[\n";

            file << "   {\n";
            file << "        \"id_transaksi\": \"" << id_transaksi << "\",\n";
            file << "        \"tanggal_transaksi\": \"" 
                 << hari << "-" << bulan << "-" << tahun << "\",\n";
            file << "        \"nama_pembeli\": \"" << nama_pembeli << "\",\n";
            file << "        \"total_belanja\": " << total_belanja << "\n";
            file << "   }\n";

            file << "]\n";
        }

        else{

            while(!isi_lama.empty() && 
                 (isi_lama.back() == '\n' || isi_lama.back() == ' ')){

                isi_lama.pop_back();
            }

            if(!isi_lama.empty() && isi_lama.back() == ']'){
                isi_lama.pop_back();
            }

            string cek_koma = isi_lama;

            cek_koma.erase(remove(cek_koma.begin(), cek_koma.end(), ' '), cek_koma.end());
            cek_koma.erase(remove(cek_koma.begin(), cek_koma.end(), '\n'), cek_koma.end());

            if(cek_koma == "["){

                file << isi_lama << "\n";
            }

            else{

                file << isi_lama << ",\n";
            }

            file << "   {\n";
            file << "        \"id_transaksi\": \"" << id_transaksi << "\",\n";
            file << "        \"tanggal_transaksi\": \"" 
                 << hari << "-" << bulan << "-" << tahun << "\",\n";
            file << "        \"nama_pembeli\": \"" << nama_pembeli << "\",\n";
            file << "        \"total_belanja\": " << total_belanja << "\n";
            file << "   }\n";

            file << "]\n";
        }

        file.close();
    }
}

void detail_transaksi_pembeli(){

    load_barang(barang, jumlah_barang);

    load_detail_transaksi(detail_transaksi, jumlah_detail_transaksi);

    load_laporan_transaksi_kasir(laporan_kasir, jumlah_laporan_kasir);

    if(jumlah_barang == 0){

        cout << "Barang belum diisi" << endl;
        cout << "Tekan Enter untuk kembali...";
        cin.get();
        return;
    }

    bool semua_barang_habis = true;

    for(int i = 0; i < jumlah_barang; i++){

        if(barang[i].stok > 0){

            semua_barang_habis = false;
            break;
        }
    }

    if(semua_barang_habis){

        cout << "Barang habis terjual!" << endl;
        cout << "Tekan Enter untuk kembali...";
        cin.get();
        return;
    }

    vector<string> daftarBarang;

    for(int i = 0; i < jumlah_barang; i++){

        string data =
            barang[i].id_barang + " | " +
            barang[i].nama_barang + " | Rp." +
            format_harga(barang[i].harga_barang) + " | Stok: " +
            to_string(barang[i].stok);

        daftarBarang.push_back(data);
    }

    string id_transaksi = generate_id_transaksi();

    int total_belanja = 0;

    while(true){

        load_barang(barang, jumlah_barang);

        daftarBarang.clear();

        bool semua_stok_habis = true;

        for(int i = 0; i < jumlah_barang; i++){

            string data =
                barang[i].id_barang + " | " +
                barang[i].nama_barang + " | Rp." +
                format_harga(barang[i].harga_barang) + " | Stok: " +
                to_string(barang[i].stok);

            daftarBarang.push_back(data);

            if(barang[i].stok > 0){
                semua_stok_habis = false;
            }
        }

        if(semua_stok_habis){

            cout << "\nSemua stok barang sudah habis!\n";

            if(total_belanja > 0){

                while(true){

                    char konfirmasi_transaksi;

                    cout << "\nApakah anda yakin ingin mengonfirmasi pesanan ini? (y/n): ";
                    cin >> konfirmasi_transaksi;
                    cin.ignore();

                    if(konfirmasi_transaksi == 'y' || konfirmasi_transaksi == 'Y'){

                        simpan_laporan_transaksi_kasir(id_transaksi, username_login, total_belanja);

                        cout << "\nTRANSAKSI BERHASIL!\n";

                        cout << "ID Transaksi : "<< id_transaksi << endl;

                        cout << "Total Belanja : Rp."<< format_harga(total_belanja) << endl;

                        ofstream hapus("Database/detail_transaksi.json");

                        if(hapus.is_open()){

                            hapus << "[]";

                            hapus.close();
                        }

                        cout << "\nTekan Enter untuk kembali...";
                        cin.get();

                        return;
                    }

                    else if(konfirmasi_transaksi == 'n' || konfirmasi_transaksi == 'N'){

                        load_barang(barang, jumlah_barang);

                        load_detail_transaksi(detail_transaksi, jumlah_detail_transaksi);

                        for(int i = 0; i < jumlah_detail_transaksi; i++){

                            for(int j = 0; j < jumlah_barang; j++){

                                if(detail_transaksi[i].id_barang == barang[j].id_barang){

                                    barang[j].stok += detail_transaksi[i].jumlah_beli;

                                    break;
                                }
                            }
                        }

                        update_barang_json(barang, jumlah_barang);

                        ofstream hapus("Database/detail_transaksi.json");

                        if(hapus.is_open()){

                            hapus << "[]";

                            hapus.close();
                        }

                        cout << "\nTransaksi dibatalkan dan stok dikembalikan!\n";

                        cout << "\nTekan Enter untuk kembali...";
                        cin.get();

                        return;
                    }

                    else{

                        cout << "Input konfirmasi tidak valid!\n";
                    }
                }
            }

            cout << "\nTekan Enter untuk kembali...";
            cin.get();

            return;
        }

        int pilih = pilihMenu(
            daftarBarang,
            "PILIH BARANG YANG INGIN DIBELI",
            false,
            true
        );

        if(barang[pilih].stok <= 0){

            cout << "Barang sedang habis!" << endl;

            cout << "Tekan Enter untuk memilih barang lain...";
            cin.get();

            continue;
        }

        int jumlah_beli;

        while(true){

            cout << "Jumlah beli : ";

            string input;
            getline(cin, input);

            try{

                jumlah_beli = stoi(input);

                if(jumlah_beli <= 0){

                    cout << "Jumlah beli tidak boleh 0 atau minus!\n";
                    continue;
                }

                if(jumlah_beli > barang[pilih].stok){

                    cout << "Stok tidak mencukupi!\n";
                    continue;
                }

                break;
            }
            catch(...){

                cout << "Jumlah beli harus angka!\n";
            }
        }

        barang[pilih].stok -= jumlah_beli;

        update_barang_json(barang, jumlah_barang);

        int subtotal = jumlah_beli * barang[pilih].harga_barang;

        total_belanja += subtotal;

        cout << "Subtotal : Rp." << format_harga(subtotal) << endl;

        Detail_Transaksi detail;

        detail.id_transaksi = id_transaksi;
        detail.id_barang = barang[pilih].id_barang;
        detail.nama_barang = barang[pilih].nama_barang;
        detail.harga_barang = barang[pilih].harga_barang;
        detail.jumlah_beli = jumlah_beli;
        detail.subtotal = subtotal;

        simpan_detail_transaksi_json(detail);

        char konfirmasi;

        while(true){

            cout << "\nTambah barang lagi? (y/n): ";
            cin >> konfirmasi;
            cin.ignore();

            if(konfirmasi == 'y' || konfirmasi == 'Y'){

                break;
            }

            else if(konfirmasi == 'n' || konfirmasi == 'N'){

                while(true){

                    char konfirmasi_transaksi;

                    cout << "\nApakah anda yakin ingin mengonfirmasi pesanan ini? (y/n): ";
                    cin >> konfirmasi_transaksi;
                    cin.ignore();

                    if(konfirmasi_transaksi == 'y' || konfirmasi_transaksi == 'Y'){

                        simpan_laporan_transaksi_kasir(id_transaksi, username_login, total_belanja);

                        cout << "\nTRANSAKSI BERHASIL!\n";

                        cout << "ID Transaksi : "<< id_transaksi << endl;

                        cout << "Total Belanja : Rp."<< format_harga(total_belanja) << endl;

                        ofstream hapus("Database/detail_transaksi.json");

                        if(hapus.is_open()){

                            hapus << "[]";

                            hapus.close();
                        }

                        cout << "\nTekan Enter untuk kembali...";
                        cin.get();

                        return;
                    }

                    else if(konfirmasi_transaksi == 'n' || konfirmasi_transaksi == 'N'){

                        load_barang(barang, jumlah_barang);

                        load_detail_transaksi(detail_transaksi, jumlah_detail_transaksi);

                        for(int i = 0; i < jumlah_detail_transaksi; i++){

                            for(int j = 0; j < jumlah_barang; j++){

                                if(detail_transaksi[i].id_barang == barang[j].id_barang){

                                    barang[j].stok +=
                                    detail_transaksi[i].jumlah_beli;

                                    break;
                                }
                            }
                        }

                        update_barang_json(barang, jumlah_barang);

                        ofstream hapus("Database/detail_transaksi.json");

                        if(hapus.is_open()){

                            hapus << "[]";

                            hapus.close();
                        }

                        cout << "\nTransaksi dibatalkan dan stok dikembalikan!\n";

                        cout << "\nTekan Enter untuk kembali...";
                        cin.get();

                        return;
                    }

                    else{

                        cout << "Input konfirmasi tidak valid!\n";
                    }
                }
            }

            else{

                cout << "Input konfirmasi tidak valid!\n";
            }
        }

        if(konfirmasi == 'y' || konfirmasi == 'Y'){

            continue;
        }
    }
}

void menu_pembeli(Barang *barang, int &jumlah_barang, User *users, int &jumlah_user){

    system("cls");
    int posisi;

    while(true){
        vector<string> daftarMenu = {
            "Menu Lihat Barang",
            "Menu Beli Barang",
            "Keluar"
        };

        posisi = pilihMenu(daftarMenu, "SELEMAT DATANG DI MINIMARKET MUHAMMAD FAJAR (MENU PEMBELI)") + 1;

        if(posisi == 1){
            lihat_barang(barang,jumlah_barang);
            cout << "\nTekan Enter untuk kembali...";
            cin.get();
        }
        else if(posisi == 2){
            detail_transaksi_pembeli();
        }
        else if(posisi == 3){
            cout<<"Anda Keluar dari MENU MINIMARKET MUHAMMAD FAJAR\n";
            cout << "\nTekan Enter untuk kembali...";
            cin.get();
            break;
        }
    }
}