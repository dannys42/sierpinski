import java.applet.Applet;
import java.awt.*;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Random;


public class Sierpinski extends Applet
{
    Timer iterateTimer;
    Timer redrawTimer;
    Point[] drawPoints;
    int drawPoints_num;
    Point[] vertices;
    Random random;
    int canvasWidth, canvasHeight;

    /* Some constants for tweaking */
    int iteratePeriod = 10;     /* [ms] Time between iterations */
    int iterateBatch = 100;     /* [ms] Points to calculate per iteration */
    int drawPeriod = 100;       /* [ms] How often to redraw window */
    int vertexRadius = 5;       /* [px] Size of radius */

    public void init()
    {
        random = new Random();
        /* Dimensions are not valid at this point */
        drawPoints = new Point[10000];
        drawPoints_num = 0;
    }
    public void start()
    {
        /* initial triangle */
        vertices = new Point[3];
        vertices[0] = new Point( getSize().width / 2, 0 );
        vertices[1] = new Point( 0, getSize().height );
        vertices[2] = new Point( getSize().width, getSize().height );

        iterateTimer = new Timer(true);
        iterateTimer.schedule(new IterateThread(), 0, 10);

        redrawTimer = new Timer(true);
        redrawTimer.schedule(new RedrawThread(), 0, 100);
    }
    public void stop()
    {
    }
    private class RedrawThread extends TimerTask {
        public void run() {
            repaint();
        }
    }
    /* This class updates the list of points that are drawn onscreen */
    private class IterateThread extends TimerTask {
        public IterateThread()
        {
            System.out.format("Constructor called\n");
        }

        public void run() {
            Point vertex_new;
            Point point_new;
            Point point_current;
            int batch;

            for(batch=0; batch < iterateBatch; batch++) {
                vertex_new = vertices[ random.nextInt( vertices.length ) ];
    
                if( drawPoints_num <= 0 ) {
                    point_current = vertex_new;
                } else {
                    point_current = drawPoints[ drawPoints_num-1 ];
                }
                point_new = new Point(
                    (vertex_new.x - point_current.x) / 2,
                    (vertex_new.y - point_current.y) / 2 );
    
                drawPoints[drawPoints_num] = point_new;
                point_current = point_new;

                drawPoints_num++;
                if( drawPoints_num >= drawPoints.length ) {
                    iterateTimer.cancel();
                    break;
                }
            }
        }
    }

    public void paint(Graphics g)
    {
        int i;
        g.setColor(Color.red);
        g.drawString("Hellow World", 20, 20);
        g.setColor(Color.blue);
        g.drawRect(0, 0, getSize().width, getSize().height);
        g.setColor(Color.green);
        g.drawRect(5, 5, 10, 10);

        /* draw all points */
        g.setColor(Color.blue);
        for(i=0; i<drawPoints_num; i++) {
            g.fillRect(drawPoints[i].x, drawPoints[i].y, 1, 1);
        }
        /* draw vertices */
        g.setColor(Color.green);
        if( vertices != null ) {
            for(i=0; i<vertices.length; i++) {
                g.fillOval( 
                    vertices[i].x - vertexRadius/2,
                    vertices[i].y - vertexRadius/2,
                    vertexRadius,
                    vertexRadius);
            }
        }
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
            new Sierpinski(), /* not being run as Applet */
            "Hello World",       /* title for frame */
            appletWidth,
            appletHeight + 26 /* allow room for Frame bar */
            );
    }
}

