
unsigned short initialize();
unsigned short selectNCPext(NWCONN_HANDLE conn, char *ncpexName);
unsigned short ncpaccess(void *sendData, WORD sendDataLength, void *recieveData, WORD *recieveDataLength);
unsigned short isnwvol(NWCONN_HANDLE conn, char *vol_name);

short          chkpath(char *ent_path, char *ext_path);
unsigned short chkvol(unsigned char *vol_name, NWCONN_HANDLE conn, unsigned short *type, unsigned short *err);
unsigned short chgdate(char *file_name, time_t *chg_date, unsigned short *err);
unsigned short chgowner(unsigned char *dir_name, unsigned char *own_name, unsigned short *err);
unsigned short settrust(unsigned char *dir_name, NWOBJ_TYPE group_kind, unsigned char *trust_name,
					unsigned short trust_mask, unsigned char *trust_right, unsigned short *err);
unsigned short setuser(unsigned char *dir_name, NWOBJ_TYPE group_kind, unsigned char *trust_name,
					unsigned short hndltrust, unsigned short *err);
unsigned short viewinfo(unsigned char *dos_name, time_t *chg_date, unsigned char *dfs_name,
					unsigned char *right_list, unsigned long *file_size, unsigned short *err);
unsigned short viewown(unsigned char *dir_name, unsigned char *own_name, unsigned short *err);
unsigned short viewtrust(unsigned char *dir_name, struct t_data **info_pp, unsigned short *err);
