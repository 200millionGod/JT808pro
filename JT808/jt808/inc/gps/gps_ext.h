/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_INCLUDE_HARDWARE_GPS_H
#define ANDROID_INCLUDE_HARDWARE_GPS_H

#ifdef __cplusplus
extern "C" {
#endif


__BEGIN_DECLS

/**
 * The id of this module
 */
#define GPS_HARDWARE_MODULE_ID "gps"


/** Milliseconds since January 1, 1970 */
typedef int64_t GpsUtcTime;

/** Maximum number of SVs for gps_sv_status_callback(). */
/** support multi-gnss for GE2 */
#define GPS_MAX_SVS 256

/** Requested operational mode for GPS operation. */
typedef uint32_t GpsPositionMode;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** Mode for running GPS standalone (no assistance). */
#define GPS_POSITION_MODE_STANDALONE    0
/** AGPS MS-Based mode. */
#define GPS_POSITION_MODE_MS_BASED      1
/** AGPS MS-Assisted mode. */
#define GPS_POSITION_MODE_MS_ASSISTED   2

/** Requested recurrence mode for GPS operation. */
typedef uint32_t GpsPositionRecurrence;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** Receive GPS fixes on a recurring basis at a specified period. */
#define GPS_POSITION_RECURRENCE_PERIODIC    0
/** Request a single shot GPS fix. */
#define GPS_POSITION_RECURRENCE_SINGLE      1

/** GPS status event values. */
typedef uint16_t GpsStatusValue;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** GPS status unknown. */
#define GPS_STATUS_NONE             0
/** GPS has begun navigating. */
#define GPS_STATUS_SESSION_BEGIN    1
/** GPS has stopped navigating. */
#define GPS_STATUS_SESSION_END      2
/** GPS has powered on but is not navigating. */
#define GPS_STATUS_ENGINE_ON        3
/** GPS is powered off. */
#define GPS_STATUS_ENGINE_OFF       4

/** Flags to indicate which values are valid in a GpsLocation. */
typedef uint16_t GpsLocationFlags;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** GpsLocation has valid latitude and longitude. */
#define GPS_LOCATION_HAS_LAT_LONG   0x0001
/** GpsLocation has valid altitude. */
#define GPS_LOCATION_HAS_ALTITUDE   0x0002
/** GpsLocation has valid speed. */
#define GPS_LOCATION_HAS_SPEED      0x0004
/** GpsLocation has valid bearing. */
#define GPS_LOCATION_HAS_BEARING    0x0008
/** GpsLocation has valid accuracy. */
#define GPS_LOCATION_HAS_ACCURACY   0x0010

/** Flags for the gps_set_capabilities callback. */

/** GPS HAL schedules fixes for GPS_POSITION_RECURRENCE_PERIODIC mode.
    If this is not set, then the framework will use 1000ms for min_interval
    and will start and call start() and stop() to schedule the GPS.
 */
#define GPS_CAPABILITY_SCHEDULING       0x0000001
/** GPS supports MS-Based AGPS mode */
#define GPS_CAPABILITY_MSB              0x0000002
/** GPS supports MS-Assisted AGPS mode */
#define GPS_CAPABILITY_MSA              0x0000004
/** GPS supports single-shot fixes */
#define GPS_CAPABILITY_SINGLE_SHOT      0x0000008
/** GPS supports on demand time injection */
#define GPS_CAPABILITY_ON_DEMAND_TIME   0x0000010
/** GPS supports Geofencing  */
#define GPS_CAPABILITY_GEOFENCING       0x0000020

/** Flags used to specify which aiding data to delete
    when calling delete_aiding_data(). */
typedef uint16_t GpsAidingData;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
#define GPS_DELETE_EPHEMERIS        0x0001
#define GPS_DELETE_ALMANAC          0x0002
#define GPS_DELETE_POSITION         0x0004
#define GPS_DELETE_TIME             0x0008
#define GPS_DELETE_IONO             0x0010
#define GPS_DELETE_UTC              0x0020
#define GPS_DELETE_HEALTH           0x0040
#define GPS_DELETE_SVDIR            0x0080
#define GPS_DELETE_SVSTEER          0x0100
#define GPS_DELETE_SADATA           0x0200
#define GPS_DELETE_RTI              0x0400
#define GPS_DELETE_CELLDB_INFO      0x8000
#define GPS_DELETE_ALL              0xFFFF
/** SPRD:AGPS support supl 2.0 cmcc case */
#define GPS_READ_XML                0x00C0
#define GPS_STOP_XML                0x01C0
#define GPS_AGPS_MOLA_ON            0x01c1
#define GPS_AGPS_MOLA_OFF           0x01c2
/** SPRD:support spreadorbit and gnss selection for GE2 */
#define GPS_CHOOSE_GPS_ONLY         0x1004
#define GPS_CHOOSE_GLONASS_ONLY     0x1008
#define GPS_CHOOSE_BDS_ONLY         0x100c
#define GPS_CHOOSE_GLONASS_GPS      0x1010
#define GPS_CHOOSE_GPS_BDS          0x1014
#define GPS_LTE_ON                  0x1018
#define GPS_LTE_OFF                 0x1020

/** AGPS type */
typedef uint16_t AGpsType;
#define AGPS_TYPE_SUPL          1
#define AGPS_TYPE_C2K           2

typedef uint16_t AGpsSetIDType;
#define AGPS_SETID_TYPE_NONE    0
#define AGPS_SETID_TYPE_IMSI    1
#define AGPS_SETID_TYPE_MSISDN  2

/**
 * String length constants
 */
#define GPS_NI_SHORT_STRING_MAXLEN      256
#define GPS_NI_LONG_STRING_MAXLEN       2048

/**
 * GpsNiType constants
 */
typedef uint32_t GpsNiType;
#define GPS_NI_TYPE_VOICE              1
#define GPS_NI_TYPE_UMTS_SUPL          2
#define GPS_NI_TYPE_UMTS_CTRL_PLANE    3

/**
 * GpsNiNotifyFlags constants
 */
typedef uint32_t GpsNiNotifyFlags;
/** NI requires notification */
#define GPS_NI_NEED_NOTIFY          0x0001
/** NI requires verification */
#define GPS_NI_NEED_VERIFY          0x0002
/** NI requires privacy override, no notification/minimal trace */
#define GPS_NI_PRIVACY_OVERRIDE     0x0004

/**
 * GPS NI responses, used to define the response in
 * NI structures
 */
typedef int GpsUserResponseType;
#define GPS_NI_RESPONSE_ACCEPT         1
#define GPS_NI_RESPONSE_DENY           2
#define GPS_NI_RESPONSE_NORESP         3

/**
 * NI data encoding scheme
 */
typedef int GpsNiEncodingType;
#define GPS_ENC_NONE                   0
#define GPS_ENC_SUPL_GSM_DEFAULT       1
#define GPS_ENC_SUPL_UTF8              2
#define GPS_ENC_SUPL_UCS2              3
#define GPS_ENC_UNKNOWN                -1

/** AGPS status event values. */
typedef uint16_t AGpsStatusValue;
/** GPS requests data connection for AGPS. */
#define GPS_REQUEST_AGPS_DATA_CONN  1
/** GPS releases the AGPS data connection. */
#define GPS_RELEASE_AGPS_DATA_CONN  2
/** AGPS data connection initiated */
#define GPS_AGPS_DATA_CONNECTED     3
/** AGPS data connection completed */
#define GPS_AGPS_DATA_CONN_DONE     4
/** AGPS data connection failed */
#define GPS_AGPS_DATA_CONN_FAILED   5

#define AGPS_REF_LOCATION_TYPE_GSM_CELLID   1
#define AGPS_REF_LOCATION_TYPE_UMTS_CELLID  2
#define AGPS_REG_LOCATION_TYPE_MAC          3

/** Network types for update_network_state "type" parameter */
#define AGPS_RIL_NETWORK_TYPE_MOBILE        0
#define AGPS_RIL_NETWORK_TYPE_WIFI          1
#define AGPS_RIL_NETWORK_TYPE_MOBILE_MMS    2
#define AGPS_RIL_NETWORK_TYPE_MOBILE_SUPL   3
#define AGPS_RIL_NETWORK_TTYPE_MOBILE_DUN   4
#define AGPS_RIL_NETWORK_TTYPE_MOBILE_HIPRI 5
#define AGPS_RIL_NETWORK_TTYPE_WIMAX        6

/**
 * Name for the GPS XTRA interface.
 */
#define GPS_XTRA_INTERFACE      "gps-xtra"

/**
 * Name for the GPS DEBUG interface.
 */
#define GPS_DEBUG_INTERFACE      "gps-debug"

/**
 * Name for the AGPS interface.
 */
#define AGPS_INTERFACE      "agps"

/**
 * Name for NI interface
 */
#define GPS_NI_INTERFACE "gps-ni"

/**
 * Name for the AGPS-RIL interface.
 */
#define AGPS_RIL_INTERFACE      "agps_ril"

/**
 * Name for the GPS_Geofencing interface.
 */
#define GPS_GEOFENCING_INTERFACE   "gps_geofencing"


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
} GpsLocation;

/** Represents the status. */
typedef struct {
    /** set to sizeof(GpsStatus) */
    size_t          size;
    GpsStatusValue status;
} GpsStatus;

/** Represents SV information. */
typedef struct {
    /** set to sizeof(GpsSvInfo) */
    size_t          size;
    /** Pseudo-random number for the SV. */
    int     prn;
    /** Signal to noise ratio. */
    float   snr;
    /** Elevation of SV in degrees. */
    float   elevation;
    /** Azimuth of SV in degrees. */
    float   azimuth;
} GpsSvInfo;

/** Represents SV status. */
/** support multi-gnss for GE2 */
typedef struct {
    /** set to sizeof(GpsSvStatus) */
    size_t          size;

    /** Number of SVs currently visible. */
    int         num_svs;

    /** Contains an array of SV information. */
    GpsSvInfo   sv_list[GPS_MAX_SVS];

    /** Represents a bit mask indicating which SVs
     * have ephemeris data.
     */
    uint32_t    ephemeris_mask[8];

    /** Represents a bit mask indicating which SVs
     * have almanac data.
     */
    uint32_t    almanac_mask[8];

    /**
     * Represents a bit mask indicating which SVs
     * were used for computing the most recent position fix.
     */
    uint32_t    used_in_fix_mask[8];
} GpsSvStatus;

__END_DECLS

#ifdef __cplusplus
}
#endif

#endif /* ANDROID_INCLUDE_HARDWARE_GPS_H */

