    #version 330 core

    out vec4 FragColor;
    uniform float barColor;

    void main() {

        // Set progress bar to red if it is increased, otherwise set it to blue
        if (gl_FragCoord.y >= 0.0  && gl_FragCoord.x >= barColor && gl_FragCoord.x <= 1920.0 && gl_FragCoord.y <= 1080)
        {
            FragColor = vec4(0.0, 0.0, 1.0, 1.0);
        } else {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    }
 