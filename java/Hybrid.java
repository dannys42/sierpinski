import java.applet.Applet;
import java.awt.Frame;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

/* This class is thanks to mindprod.com
  Source: http://mindprod.com/jgloss/applet.html#OBJECT
*/
public class Hybrid
{

    public static void fireup(
        final Applet applet,
        final String title,
        final int applicationWidth,
        final int applicationHeight )
    {
        final Frame frame = new Frame(title);
        frame.setSize( applicationWidth, applicationHeight );
        frame.addWindowListener( new WindowAdapter()
            {
                /**
                 * Handle request to shutdown.
                 * @param e event giving details of closing.
                 */
                public void windowClosing(WindowEvent e)
                {
                    applet.stop();
                    applet.destroy();
                    System.exit(0);
                }
            }
            );
        frame.add( applet );
        applet.init();
        frame.validate();
        frame.setVisible(true);
        applet.start();
    }
}
