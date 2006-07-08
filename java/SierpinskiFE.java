import java.applet.Applet;
import java.awt.*;
import javax.swing.*;



public class SierpinskiFE extends JApplet
{
    public void init()
    {
        Container content = getContentPane();
        content.setLayout(new BorderLayout());
        content.add(new Sierpinski(), java.awt.BorderLayout.CENTER);
        content.add(new JButton("Button 1"), java.awt.BorderLayout.NORTH);
        content.add(new JButton("Button 2"), java.awt.BorderLayout.SOUTH);
    }
    public void start()
    {
    }
    public void stop()
    {
    }
    public void paintComponent(Graphics g)
    {
        int i;
        g.setColor(Color.red);
        g.drawString("Hellow World", 20, 20);
        g.setColor(Color.blue);
        g.drawRect(0, 0, getSize().width, getSize().height);
        g.setColor(Color.green);
        g.drawRect(5, 5, 10, 10);
    }

    /**
      Allow this Applet to run as an application as well.
      @param args command line arguments
    */
    static int appletWidth = 640;
    static int appletHeight = 480;
    public static void main(String args[])
    {
        Hybrid.fireup(
            new SierpinskiFE(), /* not being run as Applet */
            "Hello World",       /* title for frame */
            appletWidth,
            appletHeight + 26 /* allow room for Frame bar */
            );
    }
}

