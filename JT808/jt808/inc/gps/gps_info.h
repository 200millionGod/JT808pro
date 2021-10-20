

#ifndef __GPS_INFO__
#define __GPS_INFO__

#ifdef __cplusplus
extern "C" {
#endif

/** Represents a location. */
typedef struct {
    /** set to sizeof(GpsLocation) */
    size_t          size;
    /** Contains GpsLocationFlags bits. */
    uint16_t        flags;
    /** Represents latitude in degrees. */
    double          latitude;
    /** Represents longitude in degrees. */
    double          longitude;
    /** Represents altitude in meters above the WGS 84 reference
     * ellipsoid. */
    double          altitude;
    /** Represents speed in meters per second. */
    float           speed;
    /** Represents heading in degrees. */
    float           bearing;
    /** Represents expected accuracy in meters. */
    float           accuracy;
    /** Timestamp for the location fix. */
    GpsUtcTime      timestamp;
    
    int             fix;
} GpsLocation2;



char *xj_gps_get_version();
int xj_gps_init();
int xj_gps_attach();
int xj_gps_dettach();
int xj_gps_dettach();
int xj_gps_write_location(GpsLocation *loc);
int xj_gps_write_svstatus(GpsSvStatus *svstatus);
int xj_gps_read_location(GpsLocation2 *loc);
int xj_gps_read_svstatus(GpsSvStatus *svstatus);

#ifdef __cplusplus
}
#endif

#endif
