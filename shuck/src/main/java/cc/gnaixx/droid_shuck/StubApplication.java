package cc.gnaixx.droid_shuck;

import android.app.Application;
import android.content.Context;

/**
 * 名称: StubApplication
 * 描述:
 *
 * @author xiangqing.xue
 * @date 2017/2/27
 */

public class StubApplication extends Application {

    static {
        System.loadLibrary("shuck");
    }

    public StubApplication(){
        super();
    }

    @Override
    protected native void attachBaseContext(Context base);

    @Override
    public native void onCreate();


}
