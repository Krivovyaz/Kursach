#define _CRT_SECURE_NO_WARNINGS
#include <fcntl.h>	/* open() and O_XXX flags */
#include <sys/stat.h>	/* S_IXXX flags */
#include <sys/types.h>	/* mode_t */
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include <winioctl.h>
#include "FSInfo.h"
#include "FAT.h"


void output_full_information(struct BOOT_SECTOR_AC current_boot_sector, struct FSInfo current_FSInfo)
{
	output_boot_sector(current_boot_sector);
	output_FSInfo(current_FSInfo);
}


int main(int argc, char const* argv[]) {

	printf("Please,enter the name of a direcrtory in format'F:(c:)'\n");
	char in_string[5];
	scanf("%s", in_string);
	char c1[10] = "\\\\.\\";
	strcat(c1, in_string);

	size_t newsize = strlen(c1) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, c1, _TRUNCATE); // Преобразование к wchar_t

	HANDLE current_device_descriptor = open_device(in_string, wcstring);

	struct BOOT_SECTOR_BC current_boot_sector_BC = read_boot_sector(current_device_descriptor);
	struct BOOT_SECTOR_AC current_boot_sector_AC = converting_boot_sector(current_boot_sector_BC);
	struct FSInfo current_FSInfo = read_FSInfo(current_device_descriptor);

	seek_descriptor(current_device_descriptor, current_boot_sector_AC.backup_boot_sector_location * current_boot_sector_AC.bytes_per_sector);
	output_full_information(current_boot_sector_AC, current_FSInfo);

	check_FAT_32(current_boot_sector_AC);

	if (!check_correct_FSInfo(current_FSInfo)) {
		output_FSInfo_error();
	}

	struct BOOT_SECTOR_BC backup_boot_sector_BC = read_boot_sector(current_device_descriptor);
	struct FSInfo backup_FSInfo = read_FSInfo(current_device_descriptor);

	int is_normal_boot_sector = check_boot_sector(current_boot_sector_AC);

	if (check_equal_boot_sector(current_boot_sector_BC, backup_boot_sector_BC) != 0) {
		warning_boot_sector();
	}

	if (!check_equal_FSInfo(current_FSInfo, backup_FSInfo)) {
		warning_FSInfo();
	}

	char** FATs = read_FAT(current_device_descriptor,
		current_boot_sector_AC.number_reserved_sectors * current_boot_sector_AC.bytes_per_sector,
		current_boot_sector_AC.mirror_flags,
		current_boot_sector_AC.number_FAT_copies,
		current_boot_sector_AC.number_of_sectors_per_FAT * current_boot_sector_AC.bytes_per_sector

	);

	if (!check_equal_FAT(
		FATs,
		number_of_FATs(current_boot_sector_AC.mirror_flags, current_boot_sector_AC.number_FAT_copies),
		current_boot_sector_AC.number_of_sectors_per_FAT * current_boot_sector_AC.bytes_per_sector))
	{
		error_FATs();
	}
	system("pause");
	return 0;
}
