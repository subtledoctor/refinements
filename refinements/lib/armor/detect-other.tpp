READ_SHORT 0x1c detect_other_type
vs_pierc = 0
vs_blunt = 0
vs_slash = 0
vs_missi = 0
READ_LONG 0x64 ext_off
READ_SHORT 0x68 ext_count
READ_LONG 0x6a eff_off
READ_SHORT 0x6e eq_eff_indx
READ_SHORT 0x70 eq_eff_count
READ_BYTE 0x1f fighter_unus
READ_LONG 0x50 my_unide
FOR (i = 0; (i < eq_eff_count); i+= 1) BEGIN
	READ_SHORT (eq_eff_indx + i) * 0x30 + eff_off     opcode
	READ_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 4 par1
	READ_LONG  (eq_eff_indx + i) * 0x30 + eff_off + 8 par2
	PATCH_IF (opcode = 0) THEN BEGIN
		PATCH_IF par2 = 1 THEN BEGIN
			vs_blunt = par1
		END
		PATCH_IF par2 = 2 THEN BEGIN
			vs_missi = par1
		END
		PATCH_IF par2 = 4 THEN BEGIN
			vs_pierc = par1
		END
		PATCH_IF par2 = 8 THEN BEGIN
			vs_slash = par1
		END
	END
END
PATCH_IF detect_other_type = 2 BEGIN // armor
	PATCH_IF (vs_pierc = 1 && vs_blunt = 2 && vs_slash = 0) || my_unide = 6679 THEN BEGIN
		SPRINT fromfile ~splint.txt~ SPRINT base_file ~splint.txt~ SPRINT whatis splint SET base_unide = 6679 SET base_ac = 4
	END
	PATCH_IF (vs_pierc = 0 && vs_blunt =0 - 2 && vs_slash = 2) || my_unide = 6673 THEN BEGIN
		SPRINT fromfile ~chain.txt~ SPRINT base_file ~chain.txt~ SPRINT whatis chain SET base_unide = 6673 SET base_ac = 5
	END
	PATCH_IF (vs_pierc = 0 - 2 && vs_blunt = 0 && vs_slash = 0) || my_unide = 6652 THEN BEGIN
		SPRINT fromfile ~leather.txt~ SPRINT base_file ~leather.txt~ SPRINT whatis leather SET base_unide = 6652 SET base_ac = 8
	END
	PATCH_IF (vs_pierc = 1 && vs_blunt = 0 && vs_slash = 2) || my_unide = 6667 THEN BEGIN
		SPRINT fromfile ~studded.txt~ SPRINT base_file ~studded.txt~ SPRINT whatis studded SET base_unide = 6667 SET base_ac = 7
	END
	PATCH_IF (vs_pierc = 0 && vs_blunt = 0 && vs_slash = 3) || my_unide = 6681 THEN BEGIN
		SPRINT fromfile ~plate.txt~ SPRINT base_file ~plate.txt~ SPRINT whatis plate SET base_unide = 6681 SET base_ac = 3
	END
	PATCH_IF (vs_pierc = 3 && vs_blunt = 0 && vs_slash = 4) || my_unide = 6683 THEN BEGIN
		SPRINT fromfile ~full.txt~ SPRINT base_file ~full.txt~ SPRINT whatis full SET base_unide = 6683 SET base_ac = 1
	END
	PATCH_IF base_ac = 10  && NOT ((fighter_unus BAND 8) || my_unide = (0 - 1) || (vs_pierc = 0 && vs_blunt = 0 && vs_slash = 0)) THEN BEGIN
		PATCH_PRINT ~armor %SOURCE_RES% not identified %fighter_unus%~
	END
END ELSE PATCH_IF detect_other_type = 12 BEGIN // shield
	PATCH_IF (vs_missi < 0 && vs_pierc < 0) || my_unide = 16303 THEN BEGIN
		SPRINT fromfile ~buckler.txt~ SPRINT whatis buckler SET base_unide = 16303 SET base_ac = 11
	END
	PATCH_IF (vs_missi < 0 && vs_pierc = 0) || my_unide = 6818 THEN BEGIN
		SPRINT fromfile ~small.txt~ SPRINT whatis small SET base_unide = 6818 SET base_ac = 11
	END
	PATCH_IF vs_missi = 0 || my_unide = 6819 THEN BEGIN
		SPRINT fromfile ~medium.txt~ SPRINT whatis medium SET base_unide = 6819 SET base_ac = 11
	END
	PATCH_IF vs_missi > 0 || my_unide = 6820 THEN BEGIN
		SPRINT fromfile ~large.txt~ SPRINT whatis large SET base_unide = 6820 SET base_ac = 11
	END
	PATCH_IF base_ac = 10 THEN BEGIN
		PATCH_PRINT ~armor %SOURCE_RES% not identified~
	END
END
