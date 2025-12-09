import React, { useEffect, useMemo, useRef, useState } from "react";

/**
 * Versión en **JavaScript (JSX)** — ahora también muestra **peso** además de X e Y.
 * Cambiá WS_HOST por la IP o mDNS del ESP32.
 */
const WS_HOST = "192.168.1.60"; // o "esp32.local"
const WS_URL = `ws://${WS_HOST}/ws`;

// Tamaño del lienzo (diámetro del círculo en px)
const DIAMETER = 870; // un poco más grande

// Rango esperado (en las unidades que mande el ESP)
// Ejemplo si X/Y están centradas: -100..100 (mm, por ejemplo)
const X_MIN = -100;
const X_MAX = 100;
const Y_MIN = -100;
const Y_MAX = 100;

// Si tu origen (0,0) es el CENTRO físico de la base, dejá CENTERED=true.
// Si (0,0) corresponde a ESQUINA (inferior-izquierda), poné CENTERED=false.
const CENTERED = true;

// Config de visualización del peso
const WEIGHT_UNIT = "kg";       // cambiá por "g", "N", etc.
const WEIGHT_DECIMALS = 2;       // cantidad de decimales a mostrar

// Estilos simples sin dependencias
const styles = {
  page: {
    fontFamily: "Inter, system-ui, Avenir, Helvetica, Arial, sans-serif",
    color: "#111827",
    background: "#fafafa",
    minHeight: "100vh",
    display: "grid",
    gridTemplateColumns: "320px 1fr",
    gap: 12,
    padding: 12,
  },
  panel: {
    background: "white",
    border: "1px solid #e5e7eb",
    borderRadius: 16,
    padding: 16,
    boxShadow: "0 1px 2px rgba(0,0,0,0.04)",
    height: "fit-content",
    position: "relative", // para ubicar el título absoluto sin empujar el SVG
  },
  statusBox: (connected) => ({
    display: "inline-flex",
    alignItems: "center",
    gap: 10,
    padding: "10px 12px",
    borderRadius: 10,
    background: connected ? "#ECFDF5" : "#FEF2F2",
    border: connected ? "1px solid #A7F3D0" : "1px solid #FECACA",
  }),
  statusDot: (connected) => ({
    width: 12,
    height: 12,
    borderRadius: 4,
    background: connected ? "#10B981" : "#EF4444",
  }),
  label: { color: "#6b7280", fontSize: 13 },
  value: { fontVariantNumeric: "tabular-nums", fontSize: 20, fontWeight: 600 },
  circleWrap: {
    display: "flex",
    alignItems: "center",
    justifyContent: "center",
  },
  legend: { color: "#6b7280", fontSize: 13 },
  titleAbs: { position: "absolute", top: 10, left: 16, margin: 0, fontSize: 22, fontWeight: 700 },
  pointLabel: {
    fontSize: 12,
    fill: "#111827",
    paintOrder: "stroke",
    stroke: "#ffffff",
    strokeWidth: 3,
  },
};

// Mapea valor de [min..max] a [-1..1] (centrado) o [0..1] (no centrado)
function normalize(value, min, max, centered) {
  if (max === min) return 0;
  if (centered) {
    const mid = (min + max) / 2;
    const half = (max - min) / 2;
    return (value - mid) / half; // -1..1
  }
  // no centrado: 0..1
  return (value - min) / (max - min);
}

// Limita un punto (x,y) a que quede dentro del círculo de radio r
function clampToCircle(x, y, r) {
  const d = Math.hypot(x, y);
  if (d <= r || d === 0) return { x, y };
  const k = r / d;
  return { x: x * k, y: y * k };
}

export default function App() {
  const [connected, setConnected] = useState(false);
  // pos ahora incluye peso (w). Si no llega, queda undefined.
  const [pos, setPos] = useState(null); // { x, y, w? }
  const wsRef = useRef(null);
  const retryRef = useRef(null);

  useEffect(() => {
    let cancelled = false;

    const connect = () => {
      const ws = new WebSocket(WS_URL);
      wsRef.current = ws;

      ws.onopen = () => {
        if (cancelled) return;
        setConnected(true);
      };

      ws.onmessage = (ev) => {
        try {
          const msg = JSON.parse(ev.data);
          // Aceptamos diferentes nombres de campo para peso: w, weight, peso
          if (msg && msg.type === "pos" && typeof msg.x === "number" && typeof msg.y === "number" && typeof msg.w === "number") {

            setPos({ x: msg.x, y: msg.y, w: msg.w });
          }
        } catch (e) {
          // ignorar no-JSON
        }
      };

      ws.onclose = () => {
        if (cancelled) return;
        setConnected(false);
        retryRef.current = window.setTimeout(connect, 1000); // reconectar en 1s
      };

      ws.onerror = () => {
        ws.close();
      };
    };

    connect();
    return () => {
      cancelled = true;
      if (retryRef.current) window.clearTimeout(retryRef.current);
      if (wsRef.current) wsRef.current.close();
    };
  }, []);

  // Geometría del círculo
  const geom = useMemo(() => {
    const r = DIAMETER / 2;
    return { r, cx: r, cy: r };
  }, []);

  // Convertir X/Y recibidos a coordenadas pixel dentro del círculo
  const pixel = useMemo(() => {
    if (!pos) return null;

    const { r, cx, cy } = geom;

    if (CENTERED) {
      // Normalizamos a -1..1 y escalamos por el radio (origen centro)
      const nx = normalize(pos.x, X_MIN, X_MAX, true); // -1..1
      const ny = normalize(pos.y, Y_MIN, Y_MAX, true); // -1..1

      let x = nx * r;
      let y = -ny * r; // invertir Y para SVG

      const clamped = clampToCircle(x, y, r - 8);
      return { x: clamped.x + cx, y: clamped.y + cy };
    } else {
      // No centrado: 0..1 al diámetro completo, luego desplazar a centro
      const nx = normalize(pos.x, X_MIN, X_MAX, false); // 0..1
      const ny = normalize(pos.y, Y_MIN, Y_MAX, false); // 0..1

      let x = nx * DIAMETER - geom.r; // -r..+r
      let y = (1 - ny) * DIAMETER - geom.r; // invertir Y

      const clamped = clampToCircle(x, y, geom.r - 8);
      return { x: clamped.x + geom.cx, y: clamped.y + geom.cy };
    }
  }, [pos, geom]);

  return (
    <div style={styles.page}>
      {/* PANEL IZQUIERDO (estado + lecturas) */}
      <aside style={styles.panel}>
        <div style={{ display: "flex", flexDirection: "column", gap: 16 }}>
          {/* Estado global */}
          <div style={styles.statusBox(connected)}>
            <span style={styles.statusDot(connected)} />
            <strong>{connected ? "Conectado" : "Desconectado"}</strong>
          </div>

          {/* Lecturas */}
          <div>
            <div style={styles.label}>Posición X (recibida)</div>
            <div style={styles.value}>{pos ? pos.x.toFixed(2) : "—"}</div>
          </div>
          <div>
            <div style={styles.label}>Posición Y (recibida)</div>
            <div style={styles.value}>{pos ? pos.y.toFixed(2) : "—"}</div>
          </div>
          <div>
            <div style={styles.label}>Peso (recibido)</div>
            <div style={styles.value}>
              {pos && typeof pos.w === "number"
                ? `${pos.w.toFixed(WEIGHT_DECIMALS)} ${WEIGHT_UNIT}`
                : "—"}
            </div>
          </div>

          <div style={{ marginTop: 8 }}>
            <div style={styles.legend}>
              Rango X: [{X_MIN} … {X_MAX}] · Rango Y: [{Y_MIN} … {Y_MAX}]
            </div>
            <div style={styles.legend}>Origen centrado: {CENTERED ? "sí" : "no"}</div>
          </div>
        </div>
      </aside>

      {/* LIENZO CIRCULAR (derecha) */}
      <main style={{ ...styles.panel, display: "flex", flexDirection: "column", gap: 12 }}>
        <h2 style={styles.titleAbs}>Base circular (posición X,Y)</h2>
        <div style={styles.circleWrap}>
          <svg width={DIAMETER} height={DIAMETER}>
            {/* Círculo de base */}
            <circle cx={geom.cx} cy={geom.cy} r={geom.r} fill="#ffffff" stroke="#a1a1a1ff" strokeWidth={2} />

            {/* Ejes cruz (opcionales) */}
            <line x1={geom.cx - geom.r} y1={geom.cy} x2={geom.cx + geom.r} y2={geom.cy} stroke="#818181ff" strokeDasharray="6 6" />
            <line x1={geom.cx} y1={geom.cy - geom.r} x2={geom.cx} y2={geom.cy + geom.r} stroke="#818181ff" strokeDasharray="6 6" />

            {/* Punto de posición + etiqueta de peso */}
            {pixel && (
              <>
                <circle cx={pixel.x} cy={pixel.y} r={7} fill="#111827" />
                {/* Guías */}
                <line x1={pixel.x} y1={geom.cy - geom.r} x2={pixel.x} y2={geom.cy + geom.r} stroke="#9ca3af" strokeOpacity={0.35} />
                <line x1={geom.cx - geom.r} y1={pixel.y} x2={geom.cx + geom.r} y2={pixel.y} stroke="#9ca3af" strokeOpacity={0.35} />
              </>
            )}
          </svg>
        </div>
      </main>
    </div>
  );
}

