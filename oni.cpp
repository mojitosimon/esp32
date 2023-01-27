const int max_payload_size = 32;
const int payload_size_increment = 1;
 
char a[32];
char b[32];

double pi = 3.141592653589793;

double r_terre = 6378137;

double degToRad(double deg){
  return deg * pi / 180.0;
}

double degToKm(double deg){
  return deg * 2 * pi * r_terre / 360.0;
}

char buf[32];

double distanceKm(double lat1, double long1, double lat2, double long2){
  Serial.println("==DISTANCE ==");
  Serial.println(lat1);
  Serial.println(long1);
  Serial.println(lat2);
  Serial.println(long2);
  double p1 = sin(lat1) * sin(lat2);
  double p2 = cos(lat1) * cos(lat2) * cos(long1 - long2);
  double ac = acos(p1 + p2);
  double res = ac * r_terre;

  Serial.println("p1 = ");
  Serial.println(p1);

  Serial.println("p2 = ");
  Serial.println(p2);

  // sprintf(buf, "%ld",ac);
  Serial.println("ac = ");
  Serial.println(ac);

  Serial.println("res = ");
  Serial.println(res);


  //  acos( sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(long1 - long2)) * r_terre;
  return res;
}

double distanceDeg(double lat1, double long1, double lat2, double long2){
  double lat = lat1 - lat2;
  double lng = long1 - long2;
  return sqrt(lat * lat + lng * lng);
}

void setup(){
  Serial.begin(115200);
  
  double lat1 = 48.689377;
  double long1 = 2.116137;
  // double lat2 = 49.691154; // 111335.81
  // double lat2 = 48.791154; // 11228.86
  // double lat2 = 48.741154; // 5394.17
  // double lat2 = 48.716154; // 3114.32 not good !
  double lat2 = 48.689350; // 3114.32 not good !
  double long2 = 2.116144;
  double distKm = distanceKm(degToRad(lat1), degToRad(long1), degToRad(lat2), degToRad(long2));
  double distDeg = distanceDeg(lat1, long1, lat2, long2);

  Serial.println("=================================");
  Serial.println("distance KM = ");
  Serial.println(distKm);

  Serial.println("distance DEG = ");
  Serial.println(distDeg);

  Serial.println("distance KM2 = ");
  Serial.println(degToKm(distDeg));

/*
  char *a = "48.691154 , 2.120305";
  long int lat1, lat2, long1, long2;
  sscanf(a, "%ld.%ld , %ld.%ld", &lat1, &lat2, &long1, &long2);
  double latitude1 = lat1 + ( lat2 / 1000000.0);
  double longitude1 = long1 + ( long2 / 1000000.0);

  Serial.println("latitude1 =");
  Serial.println(latitude1);
  
  Serial.println("longitude1 =");
  Serial.println(longitude1);


  char *b = "48.689125 , 2.116377";
  //char *b = "48.689377 , 2.116137";
  long int lat3, lat4, long3, long4;
  sscanf(b, "%ld.%ld , %ld.%ld", &lat3, &lat4, &long3, &long4);
  double latitude2 = lat3 + ( lat4 / 1000000.0);
  double longitude2 = long3 + ( long4 / 1000000.0);

  Serial.println("latitude2 =");
  Serial.println(latitude2);
  
  Serial.println("longitude2 =");
  Serial.println(longitude2);

  double dist = distance(degToRad(latitude1), degToRad(longitude1), degToRad(latitude2), degToRad(longitude2));

  Serial.println("distance = ");
  Serial.println(dist);

  double a1 = 48.691154;
  double a2 = 2.120305;
  double b1 = 48.689125;
  double b2 = 2.116377;
  double dist2 = distance(degToRad(a1), degToRad(a2), degToRad(b1), degToRad(b2));

  Serial.println("distance2 = ");
  Serial.println(dist2);
  */
}

void loop() {
}
