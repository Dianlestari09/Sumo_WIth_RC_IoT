# 🚗 ESP32 Web Controlled Robot Car (BTS7960 Driver)

Proyek ini adalah **robot mobil WiFi berbasis ESP32** yang dikendalikan melalui **web server lokal (Access Point Mode)**.  
Motor dikendalikan menggunakan **driver BTS7960** dengan sistem **PWM (Pulse Width Modulation)** untuk mengatur arah dan kecepatan.

---

## 🧠 Fitur Utama

- Menggunakan **ESP32** sebagai otak utama.
- Motor dikendalikan via **BTS7960 dual H-Bridge**.
- Terkoneksi melalui **WiFi Access Point (tanpa internet)**.
- Kendali menggunakan **web browser** (HP/laptop).
- Dapat bergerak:
  - ⬆️ Maju
  - ⬇️ Mundur
  - ⬅️ Kiri
  - ➡️ Kanan
  - ↗️ Serong kanan/Serong kiri
  - 🛑 Berhenti
- Mengatur **kecepatan (0–9 level)** langsung dari halaman web.

---

## ⚙️ Spesifikasi Hardware

| Komponen | Deskripsi |
|-----------|-----------|
| **Mikrokontroler** | ESP32 Devkit |
| **Motor Driver** | BTS7960 43A |
| **Motor DC** | 2x Motor DC 12V |
| **Koneksi** | WiFi (Access Point) |
| **Tegangan Operasi** | 12V (Motor), 5V (ESP32) |
| **Kendali** | Web Browser (via WiFi) |

---

## 🔌 Pin Configuration (ESP32 → BTS7960)

| Pin ESP32 | Fungsi | Keterangan |
|------------|--------|------------|
| 4  | R_EN | Enable motor kanan |
| 5  | L_EN | Enable motor kiri |
| 16 | RPWM_KANAN | PWM arah kanan maju |
| 17 | LPWM_KANAN | PWM arah kanan mundur |
| 18 | RPWM_KIRI | PWM arah kiri maju |
| 19 | LPWM_KIRI | PWM arah kiri mundur |

---

## 📡 WiFi Configuration

ESP32 akan otomatis membuat **Access Point (AP)** dengan pengaturan berikut:

```cpp
const char* ssid = "SUMO1";
const char* password = "12341234";
