#ifndef BOOT_SECTOR_H
#define BOOT_SECTOR_H

#include <string.h>
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>

#define CHAR unsighed char
#define __int1 unsigned char
#define __int2 unsigned short int
#define __int4 unsigned int
#define __int8 unsigned long long

#define BOOT_SECTOR_SIZE 512

#define media_byte_hard_disk 0xF8
#define media_byte_removable_disk 0xF0

#define INT_1 0x0001
#define INT_2 0x0002
#define INT_4 0x0004
#define INT_8 0x0008
#define INT_16 0x0010
#define INT_32 0x0020
#define INT_64 0x0040
#define INT_128 0x0080
#define INT_256 0x0100
#define INT_512 0x0200
#define INT_1024 0x0400
#define INT_2048 0x0800
#define INT_4096 0x1000

#define bool int
#define true 1
#define false 0

//BC - before converting
struct BOOT_SECTOR_BC {
	unsigned char bootstrap[3];                                  // 0-2
	unsigned char OEM_name[8];                                   // 3-10
	unsigned char bytes_per_sector[2];                           // 11-12
	unsigned char sectors_per_cluster;                           // 13
	unsigned char number_reserved_sectors[2];                    // 14-15
	unsigned char number_FAT_copies;                             // 16
	unsigned char number_root_entries[2];                        // 17-18
	unsigned char total_number_sectors_NOFAT32[2];               // 19-20
	unsigned char media_descriptor_type;                         // 21
	unsigned char number_of_sectors_per_FAT_NOFAT32[2];          // 22-23; 0000 for FAT32
	unsigned char number_of_sectors_per_track[2];                // 24-25
	unsigned char number_of_heads[2];                            // 26-27
	unsigned char number_of_hidden_sectors[4];                   // 28-31
	unsigned char total_number_of_sectors[4];                    // 32-35
	unsigned char number_of_sectors_per_FAT[4];                  // 36-39
	unsigned char mirror_flags[2];                               // 40-41
	unsigned char filesystem_version[2];                         // 42-43
	unsigned char first_cluster_root_directory[4];               // 44-47
	unsigned char filesystem_information_sector[2];              // 48-49
	unsigned char backup_boot_sector_location[2];                // 50-51
	unsigned char reserved[12];                                  // 52-63
	unsigned char locival_drive_number;                          // 64
	unsigned char reserver_for_WindowsNT;                        // 65
	unsigned char extended_signature_0x29;                       // 66
	unsigned char serial_number_partition[4];                    // 67-70
	unsigned char volume_label[11];                              // 71-81
	unsigned char filesystem_type[8];                            // 82-89
	unsigned char bootstrap_data[420];                           // 90-509
	unsigned char signature_55AA[2];                              // 510-511
};


// number_root_entries * 32 / bytes_per_sector
// data sectors - total_number_of_sectors  - number_reserved_sectors + number_of_FATs * number_of_sectors_per_FAT +

//AC - after converting
struct BOOT_SECTOR_AC {
	//char[3] bootstrap;                                 // 0-2
	unsigned char OEM_name[8];                                  // 3-10
	__int2 bytes_per_sector;                           // 11-12
	__int1 sectors_per_cluster;                        // 13
	__int2 number_reserved_sectors;                    // 14-15
	__int1 number_FAT_copies;                          // 16
	__int2 number_root_entries;                        // 17-18
	//__int2 total_number_sectors_NOFAT32;               // 19-20
	__int1 media_descriptor_type;                      // 21
	//__int2 number_of_sectors_per_FAT_NOFAT32;          // 22-23; 0000 for FAT32
	__int2 number_of_sectors_per_track;                // 24-25
	__int2 number_of_heads;                            // 26-27
	__int4 number_of_hidden_sectors;                   // 28-31
	__int4 total_number_of_sectors;                    // 32-35
	__int4 number_of_sectors_per_FAT;                  // 36-39
	__int2 mirror_flags;                               // 40-41
	//__int2 filesystem_version;                         // 42-43
	__int4 first_cluster_root_directory;               // 44-47
	//__int2 filesystem_information_sector;              // 48-49
	__int2 backup_boot_sector_location;                // 50-51
	//char[12] reserved;                                 // 52-63
	//__int1 locival_drive_number;                       // 64
	//__int1 reserver_for_WindowsNT;                     // 65
	//__int1 extended_signature_0x29;                    // 66
	//__int4 serial_number_partition;                    // 67-70
	//char[11] volume_label;                             // 71-81
	//char[8] filesystem_type;                           // 82-89
	__int2 signature_55AA;                     // 510-511
};

__int1 converting_char_to_int1(unsigned char current_number)
{
	__int1 current_int = 0;
	current_int += (__int1)(current_number);
	return current_int;
}

__int2 converting_char_to_int2(unsigned char current_number[2])
{
	__int2 current_int = 0;
	current_int += (__int2)(current_number[1] << 8);
	current_int += (__int2)(current_number[0]);
	return current_int;
}

__int4 converting_char_to_int4(unsigned char current_number[4])
{
	__int4 current_int = 0;
	current_int += (__int4)(current_number[3] << 24);
	current_int += (__int4)(current_number[2] << 16);
	current_int += (__int4)(current_number[1] << 8);
	current_int += (__int4)current_number[0];
	return current_int;
}

struct BOOT_SECTOR_AC converting_boot_sector(struct BOOT_SECTOR_BC current_boot_sector_BC)
{
	struct BOOT_SECTOR_AC current_boot_sector_AC;

	for (int i = 0; i < 8; i++) {
		current_boot_sector_AC.OEM_name[i] = current_boot_sector_BC.OEM_name[i];
	}

	current_boot_sector_AC.bytes_per_sector = converting_char_to_int2(current_boot_sector_BC.bytes_per_sector);
	current_boot_sector_AC.sectors_per_cluster = converting_char_to_int1(current_boot_sector_BC.sectors_per_cluster);
	current_boot_sector_AC.number_reserved_sectors = converting_char_to_int2(current_boot_sector_BC.number_reserved_sectors);
	current_boot_sector_AC.number_FAT_copies = converting_char_to_int1(current_boot_sector_BC.number_FAT_copies);
	current_boot_sector_AC.number_root_entries = 32;
	current_boot_sector_AC.media_descriptor_type = converting_char_to_int1(current_boot_sector_BC.media_descriptor_type);
	current_boot_sector_AC.number_of_sectors_per_track = converting_char_to_int2(current_boot_sector_BC.number_of_sectors_per_track);
	current_boot_sector_AC.number_of_heads = converting_char_to_int2(current_boot_sector_BC.number_of_heads);
	current_boot_sector_AC.number_of_hidden_sectors = converting_char_to_int4(current_boot_sector_BC.number_of_hidden_sectors);
	current_boot_sector_AC.total_number_of_sectors = converting_char_to_int4(current_boot_sector_BC.total_number_of_sectors);
	current_boot_sector_AC.number_of_sectors_per_FAT = converting_char_to_int4(current_boot_sector_BC.number_of_sectors_per_FAT);
	current_boot_sector_AC.mirror_flags = converting_char_to_int2(current_boot_sector_BC.mirror_flags);
	current_boot_sector_AC.first_cluster_root_directory = converting_char_to_int4(current_boot_sector_BC.first_cluster_root_directory);
	current_boot_sector_AC.backup_boot_sector_location = converting_char_to_int2(current_boot_sector_BC.backup_boot_sector_location);
	current_boot_sector_AC.signature_55AA = converting_char_to_int2(current_boot_sector_BC.signature_55AA);
	return current_boot_sector_AC;
}
// const char* device_path = "F:/";

void error_open_device(const char* file_path)
{
	printf("\n \'%s\' : No such directory, or cannot open your device for reading. Please check Your device,input data and try again\n", file_path);
	system("pause");
	exit(0);
}

HANDLE open_device(const char* device_path, wchar_t * wcstring)
{
	HANDLE hDevice =
		CreateFileW(wcstring, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


	if (hDevice == INVALID_HANDLE_VALUE) // невозможно открыть устройство
	{
		error_open_device(device_path);

	}
	return hDevice;
}

void error_read_device()
{
	printf("\n Cannot read memory from Your device. \n");
	system("pause");
	exit(0);
}

void seek_descriptor(HANDLE device_descriptor, __int8 offset)
{
	_LARGE_INTEGER integer;
	integer.QuadPart = offset;
	if (SetFilePointerEx(device_descriptor, integer, NULL, FILE_BEGIN) == 0) error_read_device();

}

struct BOOT_SECTOR_BC read_boot_sector(HANDLE current_device_descriptor)
{
	struct BOOT_SECTOR_BC current_boot_sector;
	if (ReadFile(current_device_descriptor, &current_boot_sector, sizeof(struct BOOT_SECTOR_BC), NULL, NULL) == 0) error_read_device();
	return current_boot_sector;
}

void warning_boot_sector()
{
	printf("WARNING in boot sector. Main boot sector does not match backup boot sector!\n");
}

void check_FAT_32(struct BOOT_SECTOR_AC current_boot_sector)
{
	long long root_dir_sectors = 1LL * current_boot_sector.number_root_entries * 32 / current_boot_sector.bytes_per_sector;
	long long data = current_boot_sector.total_number_of_sectors - (current_boot_sector.number_reserved_sectors +
		current_boot_sector.number_FAT_copies * current_boot_sector.number_of_sectors_per_FAT + root_dir_sectors);
	long long answer = data / current_boot_sector.sectors_per_cluster;
	if (answer < 4085) {
		printf("Your device is formated into FAT12 - NOT OKEY\n");
	}
	if (answer >= 4085 && answer <= 65524) {
		printf("Your device is formated into FAT16 - NOT OKEY\n");
	}
	if (answer > 65524) {
		printf("Your device is formated into FAT32 - OK!\n");
	}
}

void output_boot_sector(struct BOOT_SECTOR_AC current_boot_sector)
{
	printf("System ID \"%s\"\n", current_boot_sector.OEM_name);
	printf("Media byte \"%x\"\n", current_boot_sector.media_descriptor_type);
	if (current_boot_sector.media_descriptor_type == media_byte_hard_disk) {
		printf("Hard disk\n");
	}
	if (current_boot_sector.media_descriptor_type == media_byte_removable_disk) {
		printf("Removable disk\n");
	}
	printf("%d bytes per logical sector\n", current_boot_sector.bytes_per_sector);
	printf("%lld bytes per cluster\n", (__int8)(current_boot_sector.sectors_per_cluster * current_boot_sector.bytes_per_sector));
	printf("%d reserved sectors\n", current_boot_sector.number_reserved_sectors);
	printf("First FAT starts at byte %lld (sector %d)\n",
		(__int8)(current_boot_sector.number_reserved_sectors * current_boot_sector.bytes_per_sector),
		current_boot_sector.number_reserved_sectors
	);
	printf("%d FATs, %d bit entries\n", current_boot_sector.number_FAT_copies, current_boot_sector.number_root_entries);
	printf("%lld bytes per FAT (%d sectors)\n",
		(__int8)(current_boot_sector.number_of_sectors_per_FAT * current_boot_sector.bytes_per_sector),
		current_boot_sector.number_of_sectors_per_FAT
	);
	printf("Root directory start at cluster %d\n", current_boot_sector.first_cluster_root_directory);
	__int8 data_area_sector = (current_boot_sector.number_FAT_copies * current_boot_sector.number_of_sectors_per_FAT) +
		current_boot_sector.number_reserved_sectors;
	__int8 data_clusters = (current_boot_sector.total_number_of_sectors - current_boot_sector.number_FAT_copies *
		current_boot_sector.number_of_sectors_per_FAT - current_boot_sector.number_reserved_sectors) /
		current_boot_sector.sectors_per_cluster;
	printf("Data area starts at byte %lld (sector %d)\n", data_area_sector * current_boot_sector.bytes_per_sector, data_area_sector);
	printf("%lld data clusters (%lld bytes)\n", data_clusters, (__int8)data_clusters * current_boot_sector.sectors_per_cluster *
		current_boot_sector.bytes_per_sector);
	printf("%d sectors/track, %d heads\n", current_boot_sector.number_of_sectors_per_track, current_boot_sector.number_of_heads);
	printf("%d sectors total\n", current_boot_sector.total_number_of_sectors);
	printf("\n");
}

int check_equal_boot_sector(struct BOOT_SECTOR_BC first_boot_sector, struct BOOT_SECTOR_BC second_boot_sector)
{
	second_boot_sector.reserver_for_WindowsNT = first_boot_sector.reserver_for_WindowsNT;
	return memcmp(&first_boot_sector, &second_boot_sector, sizeof(struct BOOT_SECTOR_BC));
}

// 0 - normal 1 - warning 2 - error
int check_boot_sector(struct BOOT_SECTOR_AC current_boot_sector)
{
	int current_check = 0;
	__int1 MDT = current_boot_sector.media_descriptor_type; // MDT - media descriptor type
	if (MDT != media_byte_hard_disk && MDT != media_byte_removable_disk) {
		printf("WARNING: media descriptor type should be 0xF0(removable disk) or 0xF8(hard disk). Your device media descriptor: %x\n", MDT);
		current_check = 1;
	}
	__int1 NFC = current_boot_sector.number_FAT_copies; //NFC - number of fat copies
	if (NFC != 2) {
		printf("WARNING: number of FAT copies should be 2. Your device number of FAT copies: %d\n", NFC);
		current_check = 1;
	}
	if (current_boot_sector.signature_55AA != 0xAA55) {
		printf("WARNING: 510-511 bytes should be 0x55AA. Your device 510-511 bytes: %x\n", current_boot_sector.signature_55AA);
		current_check = 1;
	}
	__int2 BPS = current_boot_sector.bytes_per_sector; // BPS - bytes per sector
	if (BPS != INT_512 && BPS != INT_1024 && BPS != INT_2048 && BPS != INT_4096) {
		printf("ERROR: bytes per sector must be 512, 1024, 2048, 4096. From your device: %d\n", BPS);
		current_check = 2;
	}
	__int1 SPC = current_boot_sector.sectors_per_cluster; //SPC sectors per cluster
	if (SPC != INT_1 && SPC != INT_2 && SPC != INT_4 && SPC != INT_8 && SPC != INT_16 && SPC != INT_32 && SPC != INT_64 && SPC != INT_128) {
		printf("ERROR: sector per cluster must be 1, 2, 4, 8, 16, 32, 64, 128. From your device: %d\n", SPC);
		current_check = 2;
	}
	__int4 SOC = BPS * SPC; // SOC - size of clusters
	if (SOC > 65536) {
		printf("ERROR: A cluster should have at most 32768 bytes. In rare cases 65536 is OK. Your device cluster's size: %d\n", SOC);
		current_check = 2;
	}
	return current_check;
}

// 0 -FAT32, 1 - FAT 16, 2 - FAT 32

#endif BOOT_SECTOR_H

