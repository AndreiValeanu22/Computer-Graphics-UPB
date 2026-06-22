#version 330 // Versiunea GLSL

struct Light { // Structura pentru o lumina
    vec3 position; // Pozitia luminii
    vec3 color; // Culoarea luminii
    vec3 direction; // Directia pentru spotlight
    float cut_off; // Unghiul de taiere
    int type; // Tipul luminii
}; // Sfarsit struct Light

in vec2 texcoord; // UV de la vertex shader
in vec3 world_position; // Pozitia in world space
in vec3 world_normal; // Normala in world space

uniform Light lights[20]; // Vector de lumini
uniform int num_lights; // Numarul de lumini active
uniform vec3 eye_position; // Pozitia camerei
uniform vec3 object_color; // Culoarea obiectului
uniform vec3 fog_color; // Culoarea cetii
uniform float fog_max_dist; // Distanta maxima pentru ceata
uniform float fog_density; // Densitatea cetii
uniform sampler2D texture_1; // Textura principala
uniform sampler2D texture_cookie; // Textura light cookie
uniform float emissive_strength; // Intensitatea emisiei
uniform float texcoord_scale; // Multiplicator UV pentru repetare

layout(location = 0) out vec4 out_color; // Culoarea finala

void main() // Intrarea in shader
{ // Deschidere main
    vec3 N = normalize(world_normal); // Normala normalizata
    vec3 V = normalize(eye_position - world_position); // Vector spre camera
    vec3 final_light = vec3(0); // Acumulator pentru lumina

    for (int i = 0; i < num_lights; i++) { // Itereaza luminile
        vec3 L = normalize(lights[i].position - world_position); // Vector spre lumina
        float dist = distance(lights[i].position, world_position); // Distanta la lumina
        float attenuation = 1.0 / (1.0 + 0.1 * dist + 0.05 * dist * dist); // Atenuare

        // Componenta Diffuse
        float diff = max(dot(N, L), 0.0); // Iluminare Lambert
        vec3 light_contrib = lights[i].color * diff * attenuation; // Contributia luminii

        if (lights[i].type == 1) { // Spotlight
            float spot_light = dot(-L, normalize(lights[i].direction)); // Alinierea cu directia spotului
            float spot_limit = cos(lights[i].cut_off); // Pragul spotului
            
            if (spot_light > spot_limit) { // Daca este in con
                float linear_att = (spot_light - spot_limit) / (1.0 - spot_limit); // Atenuare in con
                float light_att_factor = pow(linear_att, 2.0); // Accentueaza marginea conului
                
                // Aplicare Light Cookie in spatiul spotului
                vec3 dir = normalize(lights[i].direction); // Directia spotului
                vec3 up = vec3(0, 1, 0); // Vector up global
                vec3 right = normalize(cross(dir, up)); // Vector dreapta
                vec3 up2 = normalize(cross(right, dir)); // Vector up corect
                vec3 to_point = world_position - lights[i].position; // Vector spre punct
                float z = dot(to_point, -dir); // Distanta pe axa spotului

                vec3 cookie_color = vec3(0); // Culoarea cookie initiala
                if (z > 0.001) { // Daca punctul e in fata spotului
                    float x = dot(to_point, right); // Coordonata X locala
                    float y = dot(to_point, up2); // Coordonata Y locala
                    float tan_cut = tan(lights[i].cut_off); // Tangenta unghiului
                    vec2 uv = vec2(x / z, y / z); // Proiectie pe planul z=1
                    uv = uv / (tan_cut * 2.0) + 0.5; // Mapare in [0,1]
                    if (uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0) { // Daca UV e valid
                        cookie_color = texture(texture_cookie, uv).rgb; // Esantioneaza cookie
                    } // Sfarsit validare UV
                } // Sfarsit z > 0
                
                final_light += light_contrib * light_att_factor * cookie_color; // Adauga contributia spotului
            } // Sfarsit spot in con
        } else { // Lumina punctiforma
            final_light += light_contrib; // Adauga contributia
        } // Sfarsit tip lumina
    } // Sfarsit bucla lumini

    //vec3 tex_color = texture(texture_1, texcoord).rgb;
    vec4 tex_color_raw = texture(texture_1, texcoord * texcoord_scale); // Citeste textura cu tiling
    vec3 final_tex; // Textura finala

    // Daca textura lipseste sau e neagra, foloseste culoarea setata din C++
    if (length(tex_color_raw.rgb) < 0.05) { // Verifica textura neagra
        final_tex = object_color; // Foloseste culoarea obiectului
    } else { // Daca textura este valida
        final_tex = tex_color_raw.rgb; // Foloseste culoarea texturii
    } // Sfarsit selectie textura

    // Lumina ambientala crescuta la 0.4 ca sa vezi totul clar
    vec3 color_with_light = final_tex * (final_light + 0.4); // Adauga componenta ambientala
    vec3 emissive = object_color * emissive_strength; // Emisie pentru licurici

    // Efect de Ceata (Fog)
    float d = distance(eye_position, world_position); // Distanta pana la camera
    float f_fog = 1.0 - exp(-fog_density * d); // Model de ceata exponentiala
    f_fog = clamp(f_fog, 0.0, 1.0); // Clamp la [0,1]
    vec3 final_color = mix(color_with_light + emissive, fog_color, f_fog); // Amestec cu ceata

    out_color = vec4(final_color, 1.0); // Culoarea finala
}