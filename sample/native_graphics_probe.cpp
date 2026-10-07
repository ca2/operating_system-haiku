// Native Interface Kit probe for the ca2 Haiku port.
#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <Shape.h>
#include <GradientLinear.h>
#include <Font.h>
#include <stdio.h>

class ProbeView : public BView {
public:
  ProbeView(BRect bounds) : BView(bounds, "native-graphics", B_FOLLOW_ALL, B_WILL_DRAW) {
    SetViewColor(245, 245, 245);
  }
  void Draw(BRect) override {
    SetHighColor(30, 40, 55);
    SetFontSize(20);
    DrawString("ca2: native Haiku drawing probe", BPoint(24, 40));
    BGradientLinear gradient(BPoint(24, 70), BPoint(300, 150));
    gradient.AddColor(rgb_color{45, 120, 220, 255}, 0);
    gradient.AddColor(rgb_color{50, 200, 150, 255}, 255);
    FillRoundRect(BRect(24, 70, 300, 150), 12, 12, gradient);
    SetDrawingMode(B_OP_ALPHA);
    SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
    SetHighColor(230, 80, 60, 150);
    FillEllipse(BRect(245, 95, 360, 210));
    BShape shape;
    shape.MoveTo(BPoint(24, 240));
    BPoint curve[3] = { BPoint(100, 170), BPoint(230, 310), BPoint(330, 235) };
    shape.BezierTo(curve);
    SetHighColor(30, 40, 55);
    SetPenSize(3);
    StrokeShape(&shape);
    SetFontSize(14);
    DrawString("Resize this window; close it to finish the probe.", BPoint(24, 285));
    printf("Native BView repaint: %.0f x %.0f\n", Bounds().Width()+1, Bounds().Height()+1);
    fflush(stdout);
  }
};
class ProbeWindow : public BWindow {
public:
  ProbeWindow() : BWindow(BRect(100,100,600,420), "ca2 Haiku native graphics", B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE) {
    AddChild(new ProbeView(Bounds()));
  }
};
int main() {
  BApplication app("application/x-vnd.ca2-native-graphics-probe");
  if (app.InitCheck() != B_OK) return 1;
  (new ProbeWindow())->Show();
  app.Run();
  return 0;
}
