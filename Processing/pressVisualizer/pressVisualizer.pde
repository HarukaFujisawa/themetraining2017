import processing.serial.*;

Serial myPort;

int NUM = 1;
int cnt;
color[] col = new color[6];
int inByte;

void setup()
{
  size(640, 640);
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
  
  initGraph();
}

void draw()
{
  //background(inByte);
  
  for (int i = 0; i < NUM; i++) {
    fill(col[i]);
    float tx = map(cnt, 0, width, 0, width);
    float ty = map(inByte, 0, 255, height, 0);
    ellipse(tx, ty, 4, 4);
  }

  if (cnt > width) {
    initGraph();
  }

  cnt++;

}

void serialEvent(Serial p) {
  inByte = myPort.read();
  println(inByte);
}

void initGraph() {
  background(47);
  noStroke();
  cnt = 0;

  col[0] = color(255, 127, 31);
  col[1] = color(31, 255, 127);
  col[2] = color(127, 31, 255);
  col[3] = color(31, 127, 255);
  col[4] = color(127, 255, 31);
  col[5] = color(127);
}