
#ifndef __RIL_INFO__
#define  __RIL_INFO__

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct RilInfo {
    int connected;
    int simready;
    char imei[32];
    char imsi[32];
    char iccid[32];
    int networkmode;
    int networksel;
    int rsrp;
    int cesq;
    int mcc;
    int mnc;
    int lac;
    int cid;
} RilInfo;


typedef struct RilDefSet {
    int valid;
    int ver;
    int defsim;
    int dataenable;
    int networkprefer;
    int dualsimcontrol;
    char iccidext[32];
    char iccidint[32];
    char lasticcidext[32];
    char lasticcidint[32];
    char imsiext[32];
    char imsiint[32];
    char imeiint[32];
}RilDefSet;

char * xj_ril_get_version();
int xj_ril_init();
int xj_ril_attach();
int xj_ril_dettach();
int xj_ril_deinit();
int xj_ril_write_info(RilInfo *info);
int xj_ril_read_info(RilInfo *info);
int xj_ril_set_default_sim(int external);
int xj_ril_get_default_sim();
int xj_ril_change_sim(int external);
int xj_ril_get_current_sim();
int xj_ril_is_ext_sim_inserted();
int xj_ril_is_ext_sim_changed();
int xj_ril_get_int_sim_iccid(char *iccid);
int xj_ril_get_ext_sim_iccid(char *iccid);
int xj_ril_set_int_sim_iccid(char *iccid);
int xj_ril_set_ext_sim_iccid(char *iccid);
int xj_ril_set_dual_sim_control(int set);
int xj_ril_get_dual_sim_control();
int xj_ril_get_last_int_sim_iccid(char * iccid);
int xj_ril_get_last_ext_sim_iccid(char * iccid);
int xj_ril_set_last_int_sim_iccid(char * iccid);
int xj_ril_set_last_ext_sim_iccid(char * iccid);

int xj_ril_set_int_sim_imsi(char *imsi);
int xj_ril_set_ext_sim_imsi(char *imsi);
int xj_ril_get_int_sim_imsi(char *imsi);
int xj_ril_get_ext_sim_imsi(char *imsi);

int xj_ril_set_network_prefer(int prefer);
int xj_ril_set_data_enable(int enable);
int xj_ril_get_data_enable();
int xj_ril_get_imei(char *imei);

#ifdef __cplusplus
}
#endif


#endif
