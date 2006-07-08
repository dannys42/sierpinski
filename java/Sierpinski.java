import java.applet.Applet;
import java.awt.event.*;
import java.awt.*;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Random;
import javax.swing.*;


public class Sierpinski extends JPanel
{
    Timer iterateTimer;
    Timer redrawTimer;
    Point[] drawPoints;
    int drawPoints_num;
    Point[] vertices;
    Random random;
    int canvasWidth, canvasHeight;
    boolean isInitialized = false;

    /* Some constants for tweaking */
    int iteratePeriod = 10;     /* [ms] Time between iterations */
    int iterateBatch = 100;     /* [ms] Points to calculate per iteration */
    int drawPeriod = 100;       /* [ms] How often to redraw window */
    int vertexRadius = 5;       /* [px] Size of radius */
    int vertexMargin = 25;      /* [px] how far to push vertices in from edge */

    private class componentAdapter extends java.awt.event.ComponentAdapter {
        Sierpinski parent;
        public componentAdapter(Sierpinski parent)
        {
            this.parent = parent;
        }

        public void componentResized(ComponentEvent e) {
            if( isInitialized ) {
                stop();
            }
            init();
            start();
            isInitialized = true;
        }
    }
    public Sierpinski()
    {
        setPreferredSize(new Dimension(100, 100));

        addComponentListener(new componentAdapter(this));
    }
    public Dimension getMinimumSize()
    {
        return(new Dimension(10, 10));
    }
    public Dimension getPreferredSize()
    {
        return(new Dimension(640, 480));
    }
    public void init()
    {
        random = new Random();

        /* Dimensions are not valid at this point */
        drawPoints = new Point[100000];
        drawPoints_num = 0;
    }
    public void start()
    {
        /* initial triangle */
        vertices = new Point[3];
        vertices[0] = new Point(
            getSize().width / 2,
            vertexMargin);
        vertices[1] = new Point(
            vertexMargin,
            getSize().height - vertexMargin );
        vertices[2] = new Point(
            getSize().width - vertexMargin,
            getSize().height - vertexMargin );

        iterateTimer = new Timer(true);
        iterateTimer.schedule(new IterateThread(), 0, 10);

        redrawTimer = new Timer(true);
        redrawTimer.schedule(new RedrawThread(), 0, 100);
    }
    public void stop()
    {
        iterateTimer.cancel();
        redrawTimer.cancel();
    }
    private class RedrawThread extends TimerTask {
        public void run() {
            repaint();
        }
    }
    /* This class updates the list of points that are drawn onscreen */
    private class IterateThread extends TimerTask {
        private boolean iteration()
        {
            Point vertex_new;
            Point point_new;
            Point point_current;

            vertex_new = vertices[ random.nextInt( vertices.length ) ];

            if( drawPoints_num <= 0 ) {
                point_current = vertex_new;
            } else {
                point_current = drawPoints[ drawPoints_num-1 ];
            }
            point_new = new Point(
                (vertex_new.x + point_current.x) / 2,
                (vertex_new.y + point_current.y) / 2 );

            drawPoints[drawPoints_num] = point_new;

            drawPoints_num++;
            if( drawPoints_num >= drawPoints.length ) {
                iterateTimer.cancel();
                return false;
            }
            return true;
        }

        public void run() {
            int batch;

            for(batch=0; batch < iterateBatch; batch++) {
                if( !iteration() ) {
                    break;
                }
            }
        }
    }

    protected void paintComponent(Graphics g)
    {
        int i;
        super.paintComponent(g);

        g.create();
        /* draw all points */
        g.setColor(Color.blue);
        for(i=0; i<drawPoints_num; i++) {
            g.fillRect(drawPoints[i].x, drawPoints[i].y, 1, 1);
        }
        /* draw vertices */
        g.setColor(Color.red);
        if( vertices != null ) {
            for(i=0; i<vertices.length; i++) {
                g.fillOval( 
                    vertices[i].x - vertexRadius/2,
                    vertices[i].y - vertexRadius/2,
                    vertexRadius,
                    vertexRadius);
            }
        }
        g.dispose();
    }

}

