package com.faustgames.solarsystem;

import android.opengl.GLSurfaceView;
import android.view.SurfaceHolder;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

public class GLEnginePendingOperationsController 
{
    public GLSurfaceView mGLSurfaceView = null;

    private int debugFlags;
    private int renderMode;
    
    /**
     * If we don't have a GLSurfaceView yet, then we queue up any operations that are requested, until the
     * GLSurfaceView is created.
     *
     * Initially, we created the glSurfaceView in the GLEngine constructor, and things seemed to work. However,
     * it turns out a few devices aren't set up to handle the surface related events at this point, and crash.
     *
     * This is a work around so that we can delay the creation of the GLSurfaceView until the surface is actually
     * created, so that the underlying code should be in a state to be able to handle the surface related events
     * that get fired when GLSurfaceView is created.
     */
    public List<Runnable> pendingOperations = new ArrayList<Runnable>();
    
    public GLEnginePendingOperationsController()
    {
    	
    }
    
    public void setGLWrapper(final GLSurfaceView.GLWrapper glWrapper) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.setGLWrapper(glWrapper);
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    setGLWrapper(glWrapper);
                }
            });
        }
    }

    public void setDebugFlags(final int debugFlags) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.setDebugFlags(debugFlags);
        } else {
            this.debugFlags = debugFlags;
            pendingOperations.add(new Runnable() {
                public void run() {
                    setDebugFlags(debugFlags);
                }
            });
        }
    }

    public int getDebugFlags() {
        if (mGLSurfaceView != null) {
            return mGLSurfaceView.getDebugFlags();
        } else {
            return debugFlags;
        }
    }

    public void setRenderer(final GLSurfaceView.Renderer renderer, final boolean visible) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.setRenderer(renderer);
            if (!visible) {
                mGLSurfaceView.onPause();
            }
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    setRenderer(renderer, visible);
                }
            });
        }
    }

    public void queueEvent(final Runnable r) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.queueEvent(r);
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    queueEvent(r);
                }
            });
        }
    }

    public void setEGLContextFactory(final GLSurfaceView.EGLContextFactory factory) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.setEGLContextFactory(factory);
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    setEGLContextFactory(factory);
                }
            });
        }
    }

    public void setEGLWindowSurfaceFactory(final GLSurfaceView.EGLWindowSurfaceFactory factory) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.setEGLWindowSurfaceFactory(factory);
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    setEGLWindowSurfaceFactory(factory);
                }
            });
        }
    }

    public void setEGLConfigChooser(final GLSurfaceView.EGLConfigChooser configChooser) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.setEGLConfigChooser(configChooser);
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    setEGLConfigChooser(configChooser);
                }
            });
        }
    }

    public void setEGLConfigChooser(final boolean needDepth) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.setEGLConfigChooser(needDepth);
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    setEGLConfigChooser(needDepth);
                }
            });
        }
    }

    public void setEGLConfigChooser(final int redSize, final int greenSize, final int blueSize,
        final int alphaSize, final int depthSize, final int stencilSize) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.setEGLConfigChooser(redSize, greenSize, blueSize,
                alphaSize, depthSize, stencilSize);
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    setEGLConfigChooser(redSize, greenSize, blueSize, alphaSize, depthSize, stencilSize);
                }
            });
        }
    }

    public void setEGLContextClientVersion(final int version) {
        Method method = null;

        try {
            //the setEGLContextClientVersion method is first available in api level 8, but we would
            //like to support compiling against api level 7
            method = GLSurfaceView.class.getMethod("setEGLContextClientVersion", int.class);
        } catch (NoSuchMethodException ex) {
            return;
        }

        if (mGLSurfaceView != null) {
            try {
                method.invoke(mGLSurfaceView, version);
            } catch (IllegalAccessException ex) {
                return;
            } catch (InvocationTargetException ex) {
                return;
            }
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    setEGLContextClientVersion(version);
                }
            });
        }
    }

    public void setRenderMode(final int renderMode) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.setRenderMode(renderMode);
        } else {
            this.renderMode = renderMode;
            pendingOperations.add(new Runnable() {
                public void run() {
                    setRenderMode(renderMode);
                }
            });
        }
    }

    public int getRenderMode() {
        if (mGLSurfaceView != null) {
            return mGLSurfaceView.getRenderMode();
        } else {
            return renderMode;
        }
    }

    public void requestRender() {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.requestRender();
        }
    }

    public void VisibilityChanged(final boolean visible) {
        if (mGLSurfaceView != null) {
            if (visible) {
                mGLSurfaceView.onResume();
            } else {
                mGLSurfaceView.onPause();
            }
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    if (visible) {
                        mGLSurfaceView.onResume();
                    } else {
                        mGLSurfaceView.onPause();
                    }
                }
            });
        }
    }

    public void SurfaceChanged(final SurfaceHolder holder, final int format, final int width, final int height) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.surfaceChanged(holder, format, width, height);
        } else {
            pendingOperations.add(new Runnable() {
                public void run() {
                    SurfaceChanged(holder, format, width, height);
                }
            });
        }
    }

    public void SurfaceCreated(SurfaceHolder holder, GLWallpaperService service, final GLWallpaperService.GLEngine engine) 
    {
        if (mGLSurfaceView == null) 
        {
            mGLSurfaceView = new GLSurfaceView(service)
            {
                @Override
                public SurfaceHolder getHolder() 
                {
                    return engine.getSurfaceHolder();
                }
            };
            for (Runnable pendingOperation: pendingOperations) {
                pendingOperation.run();
            }
            pendingOperations.clear();
        }
        mGLSurfaceView.surfaceCreated(holder);
    }

    public void SurfaceDestroyed(SurfaceHolder holder) {
        if (mGLSurfaceView != null) {
            mGLSurfaceView.surfaceDestroyed(holder);
        }
    }

}
