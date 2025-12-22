import { _ as yt } from "./index-COae_x0G.js";
async function wt(q = {}) {
  var _a, _b, _c, _d, _e2, _f;
  var K;
  (function() {
    var _a2;
    function e(l) {
      l = l.split("-")[0];
      for (var u = l.split(".").slice(0, 3); u.length < 3; ) u.push("00");
      return u = u.map((g, y, E) => g.padStart(2, "0")), u.join("");
    }
    var r = (l) => [l / 1e4 | 0, (l / 100 | 0) % 100, l % 100].join("."), t = 2147483647, n = typeof process < "u" && ((_a2 = process.versions) == null ? void 0 : _a2.node) ? e(process.versions.node) : t;
    if (n < 16e4) throw new Error(`This emscripten-generated code requires node v${r(16e4)} (detected v${r(n)})`);
    var i = typeof navigator < "u" && navigator.userAgent;
    if (i) {
      var o = i.includes("Safari/") && i.match(/Version\/(\d+\.?\d*\.?\d*)/) ? e(i.match(/Version\/(\d+\.?\d*\.?\d*)/)[1]) : t;
      if (o < 15e4) throw new Error(`This emscripten-generated code requires Safari v${r(15e4)} (detected v${o})`);
      var s = i.match(/Firefox\/(\d+(?:\.\d+)?)/) ? parseFloat(i.match(/Firefox\/(\d+(?:\.\d+)?)/)[1]) : t;
      if (s < 79) throw new Error(`This emscripten-generated code requires Firefox v79 (detected v${s})`);
      var d = i.match(/Chrome\/(\d+(?:\.\d+)?)/) ? parseFloat(i.match(/Chrome\/(\d+(?:\.\d+)?)/)[1]) : t;
      if (d < 85) throw new Error(`This emscripten-generated code requires Chrome v85 (detected v${d})`);
    }
  })();
  var a = q, le = !!globalThis.window, Z = !!globalThis.WorkerGlobalScope, U = ((_b = (_a = globalThis.process) == null ? void 0 : _a.versions) == null ? void 0 : _b.node) && ((_c = globalThis.process) == null ? void 0 : _c.type) != "renderer", ce = !le && !U && !Z;
  if (U) {
    const { createRequire: e } = await yt(async () => {
      const { createRequire: r } = await Promise.resolve().then(() => Ft);
      return { createRequire: r };
    }, void 0);
    var X = e(import.meta.url);
  }
  var Q = import.meta.url, x = "";
  function Je(e) {
    return a.locateFile ? a.locateFile(e, x) : x + e;
  }
  var ee, $;
  if (U) {
    if (!(((_e2 = (_d = globalThis.process) == null ? void 0 : _d.versions) == null ? void 0 : _e2.node) && ((_f = globalThis.process) == null ? void 0 : _f.type) != "renderer")) throw new Error("not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)");
    var de = X("fs");
    Q.startsWith("file:") && (x = X("path").dirname(X("url").fileURLToPath(Q)) + "/"), $ = (r) => {
      r = D(r) ? new URL(r) : r;
      var t = de.readFileSync(r);
      return c(Buffer.isBuffer(t)), t;
    }, ee = async (r, t = true) => {
      r = D(r) ? new URL(r) : r;
      var n = de.readFileSync(r, t ? void 0 : "utf8");
      return c(t ? Buffer.isBuffer(n) : typeof n == "string"), n;
    }, process.argv.length > 1 && process.argv[1].replace(/\\/g, "/"), process.argv.slice(2);
  } else if (!ce) if (le || Z) {
    try {
      x = new URL(".", Q).href;
    } catch {
    }
    if (!(globalThis.window || globalThis.WorkerGlobalScope)) throw new Error("not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)");
    Z && ($ = (e) => {
      var r = new XMLHttpRequest();
      return r.open("GET", e, false), r.responseType = "arraybuffer", r.send(null), new Uint8Array(r.response);
    }), ee = async (e) => {
      if (D(e)) return new Promise((t, n) => {
        var i = new XMLHttpRequest();
        i.open("GET", e, true), i.responseType = "arraybuffer", i.onload = () => {
          if (i.status == 200 || i.status == 0 && i.response) {
            t(i.response);
            return;
          }
          n(i.status);
        }, i.onerror = n, i.send(null);
      });
      var r = await fetch(e, { credentials: "same-origin" });
      if (r.ok) return r.arrayBuffer();
      throw new Error(r.status + " : " + r.url);
    };
  } else throw new Error("environment detection error");
  var ue = console.log.bind(console), m = console.error.bind(console);
  c(!ce, "shell environment detected but not enabled at build time.  Add `shell` to `-sENVIRONMENT` to enable.");
  var N;
  globalThis.WebAssembly || m("no native wasm support detected");
  var re = false;
  function c(e, r) {
    e || h("Assertion failed" + (r ? ": " + r : ""));
  }
  var D = (e) => e.startsWith("file://");
  function Ye() {
    var e = oe();
    c((e & 3) == 0), e == 0 && (e += 4), f[e >> 2] = 34821223, f[e + 4 >> 2] = 2310721022, f[0] = 1668509029;
  }
  function te() {
    if (!re) {
      var e = oe();
      e == 0 && (e += 4);
      var r = f[e >> 2], t = f[e + 4 >> 2];
      (r != 34821223 || t != 2310721022) && h(`Stack overflow! Stack cookie has been overwritten at ${L(e)}, expected hex dwords 0x89BACDFE and 0x2135467, but received ${L(t)} ${L(r)}`), f[0] != 1668509029 && h("Runtime error: The application has corrupted its heap memory area (address zero)!");
    }
  }
  (() => {
    var e = new Int16Array(1), r = new Int8Array(e.buffer);
    e[0] = 25459, (r[0] !== 115 || r[1] !== 99) && h("Runtime error: expected the system to be little-endian! (Run with -sSUPPORT_BIG_ENDIAN to bypass)");
  })();
  function B(e) {
    Object.getOwnPropertyDescriptor(a, e) || Object.defineProperty(a, e, { configurable: true, set() {
      h(`Attempt to set \`Module.${e}\` after it has already been processed.  This can happen, for example, when code is injected via '--post-js' rather than '--pre-js'`);
    } });
  }
  function v(e) {
    return () => c(false, `call to '${e}' via reference taken before Wasm module initialization`);
  }
  function qe(e) {
    Object.getOwnPropertyDescriptor(a, e) && h(`\`Module.${e}\` was supplied but \`${e}\` not included in INCOMING_MODULE_JS_API`);
  }
  function Ke(e) {
    return e === "FS_createPath" || e === "FS_createDataFile" || e === "FS_createPreloadedFile" || e === "FS_preloadFile" || e === "FS_unlink" || e === "addRunDependency" || e === "FS_createLazyFile" || e === "FS_createDevice" || e === "removeRunDependency";
  }
  function Ze(e) {
    fe(e);
  }
  function fe(e) {
    Object.getOwnPropertyDescriptor(a, e) || Object.defineProperty(a, e, { configurable: true, get() {
      var r = `'${e}' was not exported. add it to EXPORTED_RUNTIME_METHODS (see the Emscripten FAQ)`;
      Ke(e) && (r += ". Alternatively, forcing filesystem support (-sFORCE_FILESYSTEM) can export this for you"), h(r);
    } });
  }
  var _e, ge, V, w, H, O, G, f, pe, me, ve, he, j = false;
  function ye() {
    var e = Y.buffer;
    V = new Int8Array(e), H = new Int16Array(e), w = new Uint8Array(e), O = new Uint16Array(e), G = new Int32Array(e), f = new Uint32Array(e), pe = new Float32Array(e), me = new Float64Array(e), ve = new BigInt64Array(e), he = new BigUint64Array(e);
  }
  c(globalThis.Int32Array && globalThis.Float64Array && Int32Array.prototype.subarray && Int32Array.prototype.set, "JS engine does not provide full typed array support");
  function Xe() {
    if (a.preRun) for (typeof a.preRun == "function" && (a.preRun = [a.preRun]); a.preRun.length; ) cr(a.preRun.shift());
    B("preRun"), Fe(be);
  }
  function Qe() {
    c(!j), j = true, te(), M.__wasm_call_ctors();
  }
  function er() {
    if (te(), a.postRun) for (typeof a.postRun == "function" && (a.postRun = [a.postRun]); a.postRun.length; ) lr(a.postRun.shift());
    B("postRun"), Fe(Se);
  }
  function h(e) {
    var _a2;
    (_a2 = a.onAbort) == null ? void 0 : _a2.call(a, e), e = "Aborted(" + e + ")", m(e), re = true;
    var r = new WebAssembly.RuntimeError(e);
    throw ge == null ? void 0 : ge(r), r;
  }
  var F = { error() {
    h("Filesystem support (FS) was not included. The problem is that you are using files from JS, but files were not used from C/C++, so filesystem support was not auto-included. You can force-include filesystem support with -sFORCE_FILESYSTEM");
  }, init() {
    F.error();
  }, createDataFile() {
    F.error();
  }, createPreloadedFile() {
    F.error();
  }, createLazyFile() {
    F.error();
  }, open() {
    F.error();
  }, mkdev() {
    F.error();
  }, registerDevice() {
    F.error();
  }, analyzePath() {
    F.error();
  }, ErrnoError() {
    F.error();
  } };
  function I(e, r) {
    return (...t) => {
      c(j, `native function \`${e}\` called before runtime initialization`);
      var n = M[e];
      return c(n, `exported native function \`${e}\` not found`), c(t.length <= r, `native function \`${e}\` called with ${t.length} args but expects ${r}`), n(...t);
    };
  }
  var ne;
  function rr() {
    return a.locateFile ? Je("test.wasm") : new URL("/web-gs-editor/assets/test-CAZFaM1v.wasm", import.meta.url).href;
  }
  function tr(e) {
    if (e == ne && N) return new Uint8Array(N);
    if ($) return $(e);
    throw "both async and sync fetching of the wasm failed";
  }
  async function nr(e) {
    if (!N) try {
      var r = await ee(e);
      return new Uint8Array(r);
    } catch {
    }
    return tr(e);
  }
  async function ir(e, r) {
    try {
      var t = await nr(e), n = await WebAssembly.instantiate(t, r);
      return n;
    } catch (i) {
      m(`failed to asynchronously prepare wasm: ${i}`), D(e) && m(`warning: Loading from a file URI (${e}) is not supported in most browsers. See https://emscripten.org/docs/getting_started/FAQ.html#how-do-i-run-a-local-webserver-for-testing-why-does-my-program-stall-in-downloading-or-preparing`), h(i);
    }
  }
  async function ar(e, r, t) {
    if (!e && !D(r) && !U) try {
      var n = fetch(r, { credentials: "same-origin" }), i = await WebAssembly.instantiateStreaming(n, t);
      return i;
    } catch (o) {
      m(`wasm streaming compile failed: ${o}`), m("falling back to ArrayBuffer instantiation");
    }
    return ir(r, t);
  }
  function or() {
    var e = { env: Ge, wasi_snapshot_preview1: Ge };
    return e;
  }
  async function sr() {
    function e(s, d) {
      return M = s.exports, pt(M), ye(), M;
    }
    var r = a;
    function t(s) {
      return c(a === r, "the Module object should not be replaced during async compilation - perhaps the order of HTML elements is wrong?"), r = null, e(s.instance);
    }
    var n = or();
    if (a.instantiateWasm) return new Promise((s, d) => {
      try {
        a.instantiateWasm(n, (l, u) => {
          s(e(l, u));
        });
      } catch (l) {
        m(`Module.instantiateWasm callback failed with error: ${l}`), d(l);
      }
    });
    ne ?? (ne = rr());
    var i = await ar(N, ne, n), o = t(i);
    return o;
  }
  var Fe = (e) => {
    for (; e.length > 0; ) e.shift()(a);
  }, Se = [], lr = (e) => Se.push(e), be = [], cr = (e) => be.push(e), L = (e) => (c(typeof e == "number", `ptrToString expects a number, got ${typeof e}`), e >>>= 0, "0x" + e.toString(16).padStart(8, "0")), dr = (e) => Be(e), ur = () => He(), W = (e) => {
    W.shown || (W.shown = {}), W.shown[e] || (W.shown[e] = 1, U && (e = "warning: " + e), m(e));
  }, fr = () => h("native code called abort()"), S = (e) => {
    for (var r = ""; ; ) {
      var t = w[e++];
      if (!t) return r;
      r += String.fromCharCode(t);
    }
  }, C = {}, R = {}, z = {}, _r = class extends Error {
    constructor(r) {
      super(r), this.name = "BindingError";
    }
  }, b = (e) => {
    throw new _r(e);
  };
  function gr(e, r, t = {}) {
    var n = r.name;
    if (e || b(`type "${n}" must have a positive integer typeid pointer`), R.hasOwnProperty(e)) {
      if (t.ignoreDuplicateRegistrations) return;
      b(`Cannot register type '${n}' twice`);
    }
    if (R[e] = r, delete z[e], C.hasOwnProperty(e)) {
      var i = C[e];
      delete C[e], i.forEach((o) => o());
    }
  }
  function T(e, r, t = {}) {
    return gr(e, r, t);
  }
  var Te = (e, r, t) => {
    switch (r) {
      case 1:
        return t ? (n) => V[n] : (n) => w[n];
      case 2:
        return t ? (n) => H[n >> 1] : (n) => O[n >> 1];
      case 4:
        return t ? (n) => G[n >> 2] : (n) => f[n >> 2];
      case 8:
        return t ? (n) => ve[n >> 3] : (n) => he[n >> 3];
      default:
        throw new TypeError(`invalid integer width (${r}): ${e}`);
    }
  }, J = (e) => {
    if (e === null) return "null";
    var r = typeof e;
    return r === "object" || r === "array" || r === "function" ? e.toString() : "" + e;
  }, Ee = (e, r, t, n) => {
    if (r < t || r > n) throw new TypeError(`Passing a number "${J(r)}" from JS side to C/C++ side to an argument of type "${e}", which is outside the valid range [${t}, ${n}]!`);
  }, pr = (e, r, t, n, i) => {
    r = S(r);
    const o = n === 0n;
    let s = (d) => d;
    if (o) {
      const d = t * 8;
      s = (l) => BigInt.asUintN(d, l), i = s(i);
    }
    T(e, { name: r, fromWireType: s, toWireType: (d, l) => {
      if (typeof l == "number") l = BigInt(l);
      else if (typeof l != "bigint") throw new TypeError(`Cannot convert "${J(l)}" to ${this.name}`);
      return Ee(r, l, n, i), l;
    }, readValueFromPointer: Te(r, t, !o), destructorFunction: null });
  }, mr = (e, r, t, n) => {
    r = S(r), T(e, { name: r, fromWireType: function(i) {
      return !!i;
    }, toWireType: function(i, o) {
      return o ? t : n;
    }, readValueFromPointer: function(i) {
      return this.fromWireType(w[i]);
    }, destructorFunction: null });
  }, we = [], k = [0, 1, , 1, null, 1, true, 1, false, 1], vr = (e) => {
    e > 9 && --k[e + 1] === 0 && (c(k[e] !== void 0, "Decref for unallocated handle."), k[e] = void 0, we.push(e));
  }, ke = { toValue: (e) => (e || b(`Cannot use deleted val. handle = ${e}`), c(e === 2 || k[e] !== void 0 && e % 2 === 0, `invalid handle: ${e}`), k[e]), toHandle: (e) => {
    switch (e) {
      case void 0:
        return 2;
      case null:
        return 4;
      case true:
        return 6;
      case false:
        return 8;
      default: {
        const r = we.pop() || k.length;
        return k[r] = e, k[r + 1] = 1, r;
      }
    }
  } };
  function ie(e) {
    return this.fromWireType(f[e >> 2]);
  }
  var hr = { name: "emscripten::val", fromWireType: (e) => {
    var r = ke.toValue(e);
    return vr(e), r;
  }, toWireType: (e, r) => ke.toHandle(r), readValueFromPointer: ie, destructorFunction: null }, yr = (e) => T(e, hr), Fr = (e, r) => {
    switch (r) {
      case 4:
        return function(t) {
          return this.fromWireType(pe[t >> 2]);
        };
      case 8:
        return function(t) {
          return this.fromWireType(me[t >> 3]);
        };
      default:
        throw new TypeError(`invalid float width (${r}): ${e}`);
    }
  }, Sr = (e, r, t) => {
    r = S(r), T(e, { name: r, fromWireType: (n) => n, toWireType: (n, i) => {
      if (typeof i != "number" && typeof i != "boolean") throw new TypeError(`Cannot convert ${J(i)} to ${this.name}`);
      return i;
    }, readValueFromPointer: Fr(r, t), destructorFunction: null });
  }, br = (e, r) => Object.defineProperty(r, "name", { value: e }), Tr = (e) => {
    for (; e.length; ) {
      var r = e.pop(), t = e.pop();
      t(r);
    }
  };
  function Ae(e) {
    for (var r = 1; r < e.length; ++r) if (e[r] !== null && e[r].destructorFunction === void 0) return true;
    return false;
  }
  function Er(e, r, t, n, i) {
    if (e < r || e > t) {
      var o = r == t ? r : `${r} to ${t}`;
      i(`function ${n} called with ${e} arguments, expected ${o}`);
    }
  }
  function wr(e, r, t, n) {
    for (var i = Ae(e), o = e.length - 2, s = [], d = ["fn"], l = 0; l < o; ++l) s.push(`arg${l}`), d.push(`arg${l}Wired`);
    s = s.join(","), d = d.join(",");
    var u = `return function (${s}) {
`;
    u += `checkArgCount(arguments.length, minArgs, maxArgs, humanName, throwBindingError);
`, i && (u += `var destructors = [];
`);
    for (var g = i ? "destructors" : "null", y = ["humanName", "throwBindingError", "invoker", "fn", "runDestructors", "fromRetWire", "toClassParamWire"], l = 0; l < o; ++l) {
      var E = `toArg${l}Wire`;
      u += `var arg${l}Wired = ${E}(${g}, arg${l});
`, y.push(E);
    }
    if (u += (t || n ? "var rv = " : "") + `invoker(${d});
`, i) u += `runDestructors(destructors);
`;
    else for (var l = 2; l < e.length; ++l) {
      var P = l === 1 ? "thisWired" : "arg" + (l - 2) + "Wired";
      e[l].destructorFunction !== null && (u += `${P}_dtor(${P});
`, y.push(`${P}_dtor`));
    }
    return t && (u += `var ret = fromRetWire(rv);
return ret;
`), u += `}
`, y.push("checkArgCount", "minArgs", "maxArgs"), u = `if (arguments.length !== ${y.length}){ throw new Error(humanName + "Expected ${y.length} closure arguments " + arguments.length + " given."); }
${u}`, new Function(y, u);
  }
  function kr(e) {
    for (var r = e.length - 2, t = e.length - 1; t >= 2 && e[t].optional; --t) r--;
    return r;
  }
  function Ar(e, r, t, n, i, o) {
    var s = r.length;
    s < 2 && b("argTypes array size mismatch! Must at least get return value and 'this' types!"), c(!o, "Async bindings are only supported with JSPI.");
    for (var d = r[1] !== null && t !== null, l = Ae(r), u = !r[0].isVoid, g = s - 2, y = kr(r), E = r[0], P = r[1], _ = [e, b, n, i, Tr, E.fromWireType.bind(E), P == null ? void 0 : P.toWireType.bind(P)], p = 2; p < s; ++p) {
      var ze = r[p];
      _.push(ze.toWireType.bind(ze));
    }
    if (!l) for (var p = 2; p < r.length; ++p) r[p].destructorFunction !== null && _.push(r[p].destructorFunction);
    _.push(Er, y, g);
    var ht = wr(r, d, u, o)(..._);
    return br(e, ht);
  }
  var Pr = (e, r, t) => {
    if (e[r].overloadTable === void 0) {
      var n = e[r];
      e[r] = function(...i) {
        return e[r].overloadTable.hasOwnProperty(i.length) || b(`Function '${t}' called with an invalid number of arguments (${i.length}) - expects one of (${e[r].overloadTable})!`), e[r].overloadTable[i.length].apply(this, i);
      }, e[r].overloadTable = [], e[r].overloadTable[n.argCount] = n;
    }
  }, Ir = (e, r, t) => {
    a.hasOwnProperty(e) ? ((t === void 0 || a[e].overloadTable !== void 0 && a[e].overloadTable[t] !== void 0) && b(`Cannot register public name '${e}' twice`), Pr(a, e, e), a[e].overloadTable.hasOwnProperty(t) && b(`Cannot register multiple overloads of a function with the same number of arguments (${t})!`), a[e].overloadTable[t] = r) : (a[e] = r, a[e].argCount = t);
  }, Cr = (e, r) => {
    for (var t = [], n = 0; n < e; n++) t.push(f[r + n * 4 >> 2]);
    return t;
  }, Rr = class extends Error {
    constructor(r) {
      super(r), this.name = "InternalError";
    }
  }, Pe = (e) => {
    throw new Rr(e);
  }, Mr = (e, r, t) => {
    a.hasOwnProperty(e) || Pe("Replacing nonexistent public symbol"), a[e].overloadTable !== void 0 && t !== void 0 ? a[e].overloadTable[t] = r : (a[e] = r, a[e].argCount = t);
  }, Ie = [], Ur = (e) => {
    var r = Ie[e];
    return r || (Ie[e] = r = se.get(e)), c(se.get(e) == r, "JavaScript-side Wasm function table mirror is out of date!"), r;
  }, Nr = (e, r, t = false) => {
    c(!t, "Async bindings are only supported with JSPI."), e = S(e);
    function n() {
      var o = Ur(r);
      return o;
    }
    var i = n();
    return typeof i != "function" && b(`unknown function pointer with signature ${e}: ${r}`), i;
  };
  class Dr extends Error {
  }
  var Or = (e) => {
    var r = xe(e), t = S(r);
    return A(r), t;
  }, Lr = (e, r) => {
    var t = [], n = {};
    function i(o) {
      if (!n[o] && !R[o]) {
        if (z[o]) {
          z[o].forEach(i);
          return;
        }
        t.push(o), n[o] = true;
      }
    }
    throw r.forEach(i), new Dr(`${e}: ` + t.map(Or).join([", "]));
  }, Wr = (e, r, t) => {
    e.forEach((d) => z[d] = r);
    function n(d) {
      var l = t(d);
      l.length !== e.length && Pe("Mismatched type converter count");
      for (var u = 0; u < e.length; ++u) T(e[u], l[u]);
    }
    var i = new Array(r.length), o = [], s = 0;
    for (let [d, l] of r.entries()) R.hasOwnProperty(l) ? i[d] = R[l] : (o.push(l), C.hasOwnProperty(l) || (C[l] = []), C[l].push(() => {
      i[d] = R[l], ++s, s === o.length && n(i);
    }));
    o.length === 0 && n(i);
  }, xr = (e) => {
    e = e.trim();
    const r = e.indexOf("(");
    return r === -1 ? e : (c(e.endsWith(")"), "Parentheses for argument names should match."), e.slice(0, r));
  }, $r = (e, r, t, n, i, o, s, d) => {
    var l = Cr(r, t);
    e = S(e), e = xr(e), i = Nr(n, i, s), Ir(e, function() {
      Lr(`Cannot call ${e} due to unbound types`, l);
    }, r - 1), Wr([], l, (u) => {
      var g = [u[0], null].concat(u.slice(1));
      return Mr(e, Ar(e, g, null, i, o, s), r - 1), [];
    });
  }, Br = (e, r, t, n, i) => {
    r = S(r);
    const o = n === 0;
    let s = (l) => l;
    if (o) {
      var d = 32 - 8 * t;
      s = (l) => l << d >>> d, i = s(i);
    }
    T(e, { name: r, fromWireType: s, toWireType: (l, u) => {
      if (typeof u != "number" && typeof u != "boolean") throw new TypeError(`Cannot convert "${J(u)}" to ${r}`);
      return Ee(r, u, n, i), u;
    }, readValueFromPointer: Te(r, t, n !== 0), destructorFunction: null });
  }, Vr = (e, r, t) => {
    var n = [Int8Array, Uint8Array, Int16Array, Uint16Array, Int32Array, Uint32Array, Float32Array, Float64Array, BigInt64Array, BigUint64Array], i = n[r];
    function o(s) {
      var d = f[s >> 2], l = f[s + 4 >> 2];
      return new i(V.buffer, l, d);
    }
    t = S(t), T(e, { name: t, fromWireType: o, readValueFromPointer: o }, { ignoreDuplicateRegistrations: true });
  }, Hr = (e, r, t, n) => {
    if (c(typeof e == "string", `stringToUTF8Array expects a string (got ${typeof e})`), !(n > 0)) return 0;
    for (var i = t, o = t + n - 1, s = 0; s < e.length; ++s) {
      var d = e.codePointAt(s);
      if (d <= 127) {
        if (t >= o) break;
        r[t++] = d;
      } else if (d <= 2047) {
        if (t + 1 >= o) break;
        r[t++] = 192 | d >> 6, r[t++] = 128 | d & 63;
      } else if (d <= 65535) {
        if (t + 2 >= o) break;
        r[t++] = 224 | d >> 12, r[t++] = 128 | d >> 6 & 63, r[t++] = 128 | d & 63;
      } else {
        if (t + 3 >= o) break;
        d > 1114111 && W("Invalid Unicode code point " + L(d) + " encountered when serializing a JS string to a UTF-8 string in wasm memory! (Valid unicode code points should be in range 0-0x10FFFF)."), r[t++] = 240 | d >> 18, r[t++] = 128 | d >> 12 & 63, r[t++] = 128 | d >> 6 & 63, r[t++] = 128 | d & 63, s++;
      }
    }
    return r[t] = 0, t - i;
  }, Ce = (e, r, t) => (c(typeof t == "number", "stringToUTF8(str, outPtr, maxBytesToWrite) is missing the third parameter that specifies the length of the output buffer!"), Hr(e, w, r, t)), Re = (e) => {
    for (var r = 0, t = 0; t < e.length; ++t) {
      var n = e.charCodeAt(t);
      n <= 127 ? r++ : n <= 2047 ? r += 2 : n >= 55296 && n <= 57343 ? (r += 4, ++t) : r += 3;
    }
    return r;
  }, Me = globalThis.TextDecoder && new TextDecoder(), Ue = (e, r, t, n) => {
    var i = r + t;
    if (n) return i;
    for (; e[r] && !(r >= i); ) ++r;
    return r;
  }, Ne = (e, r = 0, t, n) => {
    var i = Ue(e, r, t, n);
    if (i - r > 16 && e.buffer && Me) return Me.decode(e.subarray(r, i));
    for (var o = ""; r < i; ) {
      var s = e[r++];
      if (!(s & 128)) {
        o += String.fromCharCode(s);
        continue;
      }
      var d = e[r++] & 63;
      if ((s & 224) == 192) {
        o += String.fromCharCode((s & 31) << 6 | d);
        continue;
      }
      var l = e[r++] & 63;
      if ((s & 240) == 224 ? s = (s & 15) << 12 | d << 6 | l : ((s & 248) != 240 && W("Invalid UTF-8 leading byte " + L(s) + " encountered when deserializing a UTF-8 string in wasm memory to a JS string!"), s = (s & 7) << 18 | d << 12 | l << 6 | e[r++] & 63), s < 65536) o += String.fromCharCode(s);
      else {
        var u = s - 65536;
        o += String.fromCharCode(55296 | u >> 10, 56320 | u & 1023);
      }
    }
    return o;
  }, De = (e, r, t) => (c(typeof e == "number", `UTF8ToString expects a number (got ${typeof e})`), e ? Ne(w, e, r, t) : ""), Gr = (e, r) => {
    r = S(r), T(e, { name: r, fromWireType(t) {
      var n = f[t >> 2], i = t + 4, o;
      return o = De(i, n, true), A(t), o;
    }, toWireType(t, n) {
      n instanceof ArrayBuffer && (n = new Uint8Array(n));
      var i, o = typeof n == "string";
      o || ArrayBuffer.isView(n) && n.BYTES_PER_ELEMENT == 1 || b("Cannot pass non-string to std::string"), o ? i = Re(n) : i = n.length;
      var s = ae(4 + i + 1), d = s + 4;
      return f[s >> 2] = i, o ? Ce(n, d, i + 1) : w.set(n, d), t !== null && t.push(A, s), s;
    }, readValueFromPointer: ie, destructorFunction(t) {
      A(t);
    } });
  }, Oe = globalThis.TextDecoder ? new TextDecoder("utf-16le") : void 0, jr = (e, r, t) => {
    c(e % 2 == 0, "Pointer passed to UTF16ToString must be aligned to two bytes!");
    var n = e >> 1, i = Ue(O, n, r / 2, t);
    if (i - n > 16 && Oe) return Oe.decode(O.subarray(n, i));
    for (var o = "", s = n; s < i; ++s) {
      var d = O[s];
      o += String.fromCharCode(d);
    }
    return o;
  }, zr = (e, r, t) => {
    if (c(r % 2 == 0, "Pointer passed to stringToUTF16 must be aligned to two bytes!"), c(typeof t == "number", "stringToUTF16(str, outPtr, maxBytesToWrite) is missing the third parameter that specifies the length of the output buffer!"), t ?? (t = 2147483647), t < 2) return 0;
    t -= 2;
    for (var n = r, i = t < e.length * 2 ? t / 2 : e.length, o = 0; o < i; ++o) {
      var s = e.charCodeAt(o);
      H[r >> 1] = s, r += 2;
    }
    return H[r >> 1] = 0, r - n;
  }, Jr = (e) => e.length * 2, Yr = (e, r, t) => {
    c(e % 4 == 0, "Pointer passed to UTF32ToString must be aligned to four bytes!");
    for (var n = "", i = e >> 2, o = 0; !(o >= r / 4); o++) {
      var s = f[i + o];
      if (!s && !t) break;
      n += String.fromCodePoint(s);
    }
    return n;
  }, qr = (e, r, t) => {
    if (c(r % 4 == 0, "Pointer passed to stringToUTF32 must be aligned to four bytes!"), c(typeof t == "number", "stringToUTF32(str, outPtr, maxBytesToWrite) is missing the third parameter that specifies the length of the output buffer!"), t ?? (t = 2147483647), t < 4) return 0;
    for (var n = r, i = n + t - 4, o = 0; o < e.length; ++o) {
      var s = e.codePointAt(o);
      if (s > 65535 && o++, G[r >> 2] = s, r += 4, r + 4 > i) break;
    }
    return G[r >> 2] = 0, r - n;
  }, Kr = (e) => {
    for (var r = 0, t = 0; t < e.length; ++t) {
      var n = e.codePointAt(t);
      n > 65535 && t++, r += 4;
    }
    return r;
  }, Zr = (e, r, t) => {
    t = S(t);
    var n, i, o;
    r === 2 ? (n = jr, i = zr, o = Jr) : (c(r === 4, "only 2-byte and 4-byte strings are currently supported"), n = Yr, i = qr, o = Kr), T(e, { name: t, fromWireType: (s) => {
      var d = f[s >> 2], l = n(s + 4, d * r, true);
      return A(s), l;
    }, toWireType: (s, d) => {
      typeof d != "string" && b(`Cannot pass non-string to C++ string type ${t}`);
      var l = o(d), u = ae(4 + l + r);
      return f[u >> 2] = l / r, i(d, u + 4, l + r), s !== null && s.push(A, u), u;
    }, readValueFromPointer: ie, destructorFunction(s) {
      A(s);
    } });
  }, Xr = (e, r) => {
    r = S(r), T(e, { isVoid: true, name: r, fromWireType: () => {
    }, toWireType: (t, n) => {
    } });
  }, Qr = () => 2147483648, et = (e, r) => (c(r, "alignment argument is required"), Math.ceil(e / r) * r), rt = (e) => {
    var r = Y.buffer.byteLength, t = (e - r + 65535) / 65536 | 0;
    try {
      return Y.grow(t), ye(), 1;
    } catch (n) {
      m(`growMemory: Attempted to grow heap from ${r} bytes to ${e} bytes, but got error: ${n}`);
    }
  }, tt = (e) => {
    var r = w.length;
    e >>>= 0, c(e > r);
    var t = Qr();
    if (e > t) return m(`Cannot enlarge memory, requested ${e} bytes, but the limit is ${t} bytes!`), false;
    for (var n = 1; n <= 4; n *= 2) {
      var i = r * (1 + 0.2 / n);
      i = Math.min(i, e + 100663296);
      var o = Math.min(t, et(Math.max(e, i), 65536)), s = rt(o);
      if (s) return true;
    }
    return m(`Failed to grow the heap from ${r} bytes to ${o} bytes, not enough memory!`), false;
  }, nt = (e) => {
    h("fd_close called without SYSCALLS_REQUIRE_FILESYSTEM");
  };
  function it(e, r, t, n) {
    return 70;
  }
  var at = [null, [], []], ot = (e, r) => {
    var t = at[e];
    c(t), r === 0 || r === 10 ? ((e === 1 ? ue : m)(Ne(t)), t.length = 0) : t.push(r);
  }, st = (e, r, t, n) => {
    for (var i = 0, o = 0; o < t; o++) {
      var s = f[r >> 2], d = f[r + 4 >> 2];
      r += 8;
      for (var l = 0; l < d; l++) ot(e, w[s + l]);
      i += d;
    }
    return f[n >> 2] = i, 0;
  }, lt = (e) => {
    var r = a["_" + e];
    return c(r, "Cannot call unknown function " + e + ", make sure it is exported"), r;
  }, ct = (e, r) => {
    c(e.length >= 0, "writeArrayToMemory array must have a length (should be an array or typed array)"), V.set(e, r);
  }, Le = (e) => Ve(e), dt = (e) => {
    var r = Re(e) + 1, t = Le(r);
    return Ce(e, t, r), t;
  }, We = (e, r, t, n, i) => {
    var o = { string: (_) => {
      var p = 0;
      return _ != null && _ !== 0 && (p = dt(_)), p;
    }, array: (_) => {
      var p = Le(_.length);
      return ct(_, p), p;
    } };
    function s(_) {
      return r === "string" ? De(_) : r === "boolean" ? !!_ : _;
    }
    var d = lt(e), l = [], u = 0;
    if (c(r !== "array", 'Return type should not be "array".'), n) for (var g = 0; g < n.length; g++) {
      var y = o[t[g]];
      y ? (u === 0 && (u = ur()), l[g] = y(n[g])) : l[g] = n[g];
    }
    var E = d(...l);
    function P(_) {
      return u !== 0 && dr(u), s(_);
    }
    return E = P(E), E;
  }, ut = (e, r, t, n) => (...i) => We(e, r, t, i);
  c(k.length === 10);
  {
    if (a.noExitRuntime && a.noExitRuntime, a.print && (ue = a.print), a.printErr && (m = a.printErr), a.wasmBinary && (N = a.wasmBinary), a.FS_createDataFile = F.createDataFile, a.FS_createPreloadedFile = F.createPreloadedFile, gt(), a.arguments && a.arguments, a.thisProgram && a.thisProgram, c(typeof a.memoryInitializerPrefixURL > "u", "Module.memoryInitializerPrefixURL option was removed, use Module.locateFile instead"), c(typeof a.pthreadMainPrefixURL > "u", "Module.pthreadMainPrefixURL option was removed, use Module.locateFile instead"), c(typeof a.cdInitializerPrefixURL > "u", "Module.cdInitializerPrefixURL option was removed, use Module.locateFile instead"), c(typeof a.filePackagePrefixURL > "u", "Module.filePackagePrefixURL option was removed, use Module.locateFile instead"), c(typeof a.read > "u", "Module.read option was removed"), c(typeof a.readAsync > "u", "Module.readAsync option was removed (modify readAsync in JS)"), c(typeof a.readBinary > "u", "Module.readBinary option was removed (modify readBinary in JS)"), c(typeof a.setWindowTitle > "u", "Module.setWindowTitle option was removed (modify emscripten_set_window_title in JS)"), c(typeof a.TOTAL_MEMORY > "u", "Module.TOTAL_MEMORY has been renamed Module.INITIAL_MEMORY"), c(typeof a.ENVIRONMENT > "u", "Module.ENVIRONMENT has been deprecated. To force the environment, use the ENVIRONMENT compile-time option (for example, -sENVIRONMENT=web or -sENVIRONMENT=node)"), c(typeof a.STACK_SIZE > "u", "STACK_SIZE can no longer be set at runtime.  Use -sSTACK_SIZE at link time"), c(typeof a.wasmMemory > "u", "Use of `wasmMemory` detected.  Use -sIMPORTED_MEMORY to define wasmMemory externally"), c(typeof a.INITIAL_MEMORY > "u", "Detected runtime INITIAL_MEMORY setting.  Use -sIMPORTED_MEMORY to define wasmMemory dynamically"), a.preInit) for (typeof a.preInit == "function" && (a.preInit = [a.preInit]); a.preInit.length > 0; ) a.preInit.shift()();
    B("preInit");
  }
  a.ccall = We, a.cwrap = ut;
  var ft = ["writeI53ToI64", "writeI53ToI64Clamped", "writeI53ToI64Signaling", "writeI53ToU64Clamped", "writeI53ToU64Signaling", "readI53FromI64", "readI53FromU64", "convertI32PairToI53", "convertI32PairToI53Checked", "convertU32PairToI53", "getTempRet0", "setTempRet0", "zeroMemory", "exitJS", "withStackSave", "strError", "inetPton4", "inetNtop4", "inetPton6", "inetNtop6", "readSockaddr", "writeSockaddr", "readEmAsmArgs", "jstoi_q", "getExecutableName", "autoResumeAudioContext", "getDynCaller", "dynCall", "handleException", "keepRuntimeAlive", "runtimeKeepalivePush", "runtimeKeepalivePop", "callUserCallback", "maybeExit", "asyncLoad", "asmjsMangle", "mmapAlloc", "HandleAllocator", "getUniqueRunDependency", "addRunDependency", "removeRunDependency", "addOnInit", "addOnPostCtor", "addOnPreMain", "addOnExit", "STACK_SIZE", "STACK_ALIGN", "POINTER_SIZE", "ASSERTIONS", "convertJsFunctionToWasm", "getEmptyTableSlot", "updateTableMap", "getFunctionAddress", "addFunction", "removeFunction", "intArrayFromString", "intArrayToString", "stringToAscii", "stringToNewUTF8", "registerKeyEventCallback", "maybeCStringToJsString", "findEventTarget", "getBoundingClientRect", "fillMouseEventData", "registerMouseEventCallback", "registerWheelEventCallback", "registerUiEventCallback", "registerFocusEventCallback", "fillDeviceOrientationEventData", "registerDeviceOrientationEventCallback", "fillDeviceMotionEventData", "registerDeviceMotionEventCallback", "screenOrientation", "fillOrientationChangeEventData", "registerOrientationChangeEventCallback", "fillFullscreenChangeEventData", "registerFullscreenChangeEventCallback", "JSEvents_requestFullscreen", "JSEvents_resizeCanvasForFullscreen", "registerRestoreOldStyle", "hideEverythingExceptGivenElement", "restoreHiddenElements", "setLetterbox", "softFullscreenResizeWebGLRenderTarget", "doRequestFullscreen", "fillPointerlockChangeEventData", "registerPointerlockChangeEventCallback", "registerPointerlockErrorEventCallback", "requestPointerLock", "fillVisibilityChangeEventData", "registerVisibilityChangeEventCallback", "registerTouchEventCallback", "fillGamepadEventData", "registerGamepadEventCallback", "registerBeforeUnloadEventCallback", "fillBatteryEventData", "registerBatteryEventCallback", "setCanvasElementSize", "getCanvasElementSize", "jsStackTrace", "getCallstack", "convertPCtoSourceLocation", "getEnvStrings", "checkWasiClock", "wasiRightsToMuslOFlags", "wasiOFlagsToMuslOFlags", "initRandomFill", "randomFill", "safeSetTimeout", "setImmediateWrapped", "safeRequestAnimationFrame", "clearImmediateWrapped", "registerPostMainLoop", "registerPreMainLoop", "getPromise", "makePromise", "idsToPromises", "makePromiseCallback", "ExceptionInfo", "findMatchingCatch", "Browser_asyncPrepareDataCounter", "isLeapYear", "ydayFromDate", "arraySum", "addDays", "getSocketFromFD", "getSocketAddress", "FS_createPreloadedFile", "FS_preloadFile", "FS_modeStringToFlags", "FS_getMode", "FS_stdin_getChar", "FS_mkdirTree", "_setNetworkCallback", "heapObjectForWebGLType", "toTypedArrayIndex", "webgl_enable_ANGLE_instanced_arrays", "webgl_enable_OES_vertex_array_object", "webgl_enable_WEBGL_draw_buffers", "webgl_enable_WEBGL_multi_draw", "webgl_enable_EXT_polygon_offset_clamp", "webgl_enable_EXT_clip_control", "webgl_enable_WEBGL_polygon_mode", "emscriptenWebGLGet", "computeUnpackAlignedImageSize", "colorChannelsInGlTextureFormat", "emscriptenWebGLGetTexPixelData", "emscriptenWebGLGetUniform", "webglGetUniformLocation", "webglPrepareUniformLocationsBeforeFirstUse", "webglGetLeftBracePos", "emscriptenWebGLGetVertexAttrib", "__glGetActiveAttribOrUniform", "writeGLArray", "registerWebGlEventCallback", "runAndAbortIfError", "ALLOC_NORMAL", "ALLOC_STACK", "allocate", "writeStringToMemory", "writeAsciiToMemory", "allocateUTF8", "allocateUTF8OnStack", "demangle", "stackTrace", "getNativeTypeSize", "getFunctionArgsName", "requireRegisteredType", "createJsInvokerSignature", "PureVirtualError", "getBasestPointer", "registerInheritedInstance", "unregisterInheritedInstance", "getInheritedInstance", "getInheritedInstanceCount", "getLiveInheritedInstances", "enumReadValueFromPointer", "genericPointerToWireType", "constNoSmartPtrRawPointerToWireType", "nonConstNoSmartPtrRawPointerToWireType", "init_RegisteredPointer", "RegisteredPointer", "RegisteredPointer_fromWireType", "runDestructor", "releaseClassHandle", "detachFinalizer", "attachFinalizer", "makeClassHandle", "init_ClassHandle", "ClassHandle", "throwInstanceAlreadyDeleted", "flushPendingDeletes", "setDelayFunction", "RegisteredClass", "shallowCopyInternalPointer", "downcastPointer", "upcastPointer", "validateThis", "char_0", "char_9", "makeLegalFunctionName", "count_emval_handles", "getStringOrSymbol", "emval_returnValue", "emval_lookupTypes", "emval_addMethodCaller"];
  ft.forEach(Ze);
  var _t = ["run", "out", "err", "callMain", "abort", "wasmExports", "HEAPF32", "HEAPF64", "HEAP8", "HEAPU8", "HEAP16", "HEAPU16", "HEAP32", "HEAPU32", "HEAP64", "HEAPU64", "writeStackCookie", "checkStackCookie", "INT53_MAX", "INT53_MIN", "bigintToI53Checked", "stackSave", "stackRestore", "stackAlloc", "createNamedFunction", "ptrToString", "getHeapMax", "growMemory", "ENV", "ERRNO_CODES", "DNS", "Protocols", "Sockets", "timers", "warnOnce", "readEmAsmArgsArray", "alignMemory", "wasmTable", "wasmMemory", "noExitRuntime", "addOnPreRun", "addOnPostRun", "freeTableIndexes", "functionsInTableMap", "setValue", "getValue", "PATH", "PATH_FS", "UTF8Decoder", "UTF8ArrayToString", "UTF8ToString", "stringToUTF8Array", "stringToUTF8", "lengthBytesUTF8", "AsciiToString", "UTF16Decoder", "UTF16ToString", "stringToUTF16", "lengthBytesUTF16", "UTF32ToString", "stringToUTF32", "lengthBytesUTF32", "stringToUTF8OnStack", "writeArrayToMemory", "JSEvents", "specialHTMLTargets", "findCanvasEventTarget", "currentFullscreenStrategy", "restoreOldWindowedStyle", "UNWIND_CACHE", "ExitStatus", "flush_NO_FILESYSTEM", "emSetImmediate", "emClearImmediate_deps", "emClearImmediate", "promiseMap", "uncaughtExceptionCount", "exceptionLast", "exceptionCaught", "Browser", "requestFullscreen", "requestFullScreen", "setCanvasSize", "getUserMedia", "createContext", "getPreloadedImageData__data", "wget", "MONTH_DAYS_REGULAR", "MONTH_DAYS_LEAP", "MONTH_DAYS_REGULAR_CUMULATIVE", "MONTH_DAYS_LEAP_CUMULATIVE", "SYSCALLS", "preloadPlugins", "FS_stdin_getChar_buffer", "FS_unlink", "FS_createPath", "FS_createDevice", "FS_readFile", "FS", "FS_root", "FS_mounts", "FS_devices", "FS_streams", "FS_nextInode", "FS_nameTable", "FS_currentPath", "FS_initialized", "FS_ignorePermissions", "FS_filesystems", "FS_syncFSRequests", "FS_readFiles", "FS_lookupPath", "FS_getPath", "FS_hashName", "FS_hashAddNode", "FS_hashRemoveNode", "FS_lookupNode", "FS_createNode", "FS_destroyNode", "FS_isRoot", "FS_isMountpoint", "FS_isFile", "FS_isDir", "FS_isLink", "FS_isChrdev", "FS_isBlkdev", "FS_isFIFO", "FS_isSocket", "FS_flagsToPermissionString", "FS_nodePermissions", "FS_mayLookup", "FS_mayCreate", "FS_mayDelete", "FS_mayOpen", "FS_checkOpExists", "FS_nextfd", "FS_getStreamChecked", "FS_getStream", "FS_createStream", "FS_closeStream", "FS_dupStream", "FS_doSetAttr", "FS_chrdev_stream_ops", "FS_major", "FS_minor", "FS_makedev", "FS_registerDevice", "FS_getDevice", "FS_getMounts", "FS_syncfs", "FS_mount", "FS_unmount", "FS_lookup", "FS_mknod", "FS_statfs", "FS_statfsStream", "FS_statfsNode", "FS_create", "FS_mkdir", "FS_mkdev", "FS_symlink", "FS_rename", "FS_rmdir", "FS_readdir", "FS_readlink", "FS_stat", "FS_fstat", "FS_lstat", "FS_doChmod", "FS_chmod", "FS_lchmod", "FS_fchmod", "FS_doChown", "FS_chown", "FS_lchown", "FS_fchown", "FS_doTruncate", "FS_truncate", "FS_ftruncate", "FS_utime", "FS_open", "FS_close", "FS_isClosed", "FS_llseek", "FS_read", "FS_write", "FS_mmap", "FS_msync", "FS_ioctl", "FS_writeFile", "FS_cwd", "FS_chdir", "FS_createDefaultDirectories", "FS_createDefaultDevices", "FS_createSpecialDirectories", "FS_createStandardStreams", "FS_staticInit", "FS_init", "FS_quit", "FS_findObject", "FS_analyzePath", "FS_createFile", "FS_createDataFile", "FS_forceLoadFile", "FS_createLazyFile", "FS_absolutePath", "FS_createFolder", "FS_createLink", "FS_joinPath", "FS_mmapAlloc", "FS_standardizePath", "MEMFS", "TTY", "PIPEFS", "SOCKFS", "tempFixedLengthArray", "miniTempWebGLFloatBuffers", "miniTempWebGLIntBuffers", "GL", "AL", "GLUT", "EGL", "GLEW", "IDBStore", "SDL", "SDL_gfx", "print", "printErr", "jstoi_s", "InternalError", "BindingError", "throwInternalError", "throwBindingError", "registeredTypes", "awaitingDependencies", "typeDependencies", "tupleRegistrations", "structRegistrations", "sharedRegisterType", "whenDependentTypesAreResolved", "getTypeName", "getFunctionName", "heap32VectorToArray", "usesDestructorStack", "checkArgCount", "getRequiredArgCount", "createJsInvoker", "UnboundTypeError", "EmValType", "EmValOptionalType", "throwUnboundTypeError", "ensureOverloadTable", "exposePublicSymbol", "replacePublicSymbol", "embindRepr", "registeredInstances", "registeredPointers", "registerType", "integerReadValueFromPointer", "floatReadValueFromPointer", "assertIntegerRange", "readPointer", "runDestructors", "craftInvokerFunction", "embind__requireFunction", "finalizationRegistry", "detachFinalizer_deps", "deletionQueue", "delayFunction", "emval_freelist", "emval_handles", "emval_symbols", "Emval", "emval_methodCallers"];
  _t.forEach(fe);
  function gt() {
    qe("fetchSettings");
  }
  var xe = v("___getTypeName");
  a._add = v("_add"), a._multiply = v("_multiply"), a.__Z5greetRKNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE = v("__Z5greetRKNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE");
  var ae = v("_malloc"), oe = v("_emscripten_stack_get_end"), A = v("_free"), $e = v("_emscripten_stack_init"), Be = v("__emscripten_stack_restore"), Ve = v("__emscripten_stack_alloc"), He = v("_emscripten_stack_get_current"), Y = v("wasmMemory"), se = v("wasmTable");
  function pt(e) {
    c(typeof e.__getTypeName < "u", "missing Wasm export: __getTypeName"), c(typeof e.add < "u", "missing Wasm export: add"), c(typeof e.multiply < "u", "missing Wasm export: multiply"), c(typeof e._Z5greetRKNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE < "u", "missing Wasm export: _Z5greetRKNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE"), c(typeof e.malloc < "u", "missing Wasm export: malloc"), c(typeof e.fflush < "u", "missing Wasm export: fflush"), c(typeof e.emscripten_stack_get_end < "u", "missing Wasm export: emscripten_stack_get_end"), c(typeof e.emscripten_stack_get_base < "u", "missing Wasm export: emscripten_stack_get_base"), c(typeof e.strerror < "u", "missing Wasm export: strerror"), c(typeof e.free < "u", "missing Wasm export: free"), c(typeof e.emscripten_stack_init < "u", "missing Wasm export: emscripten_stack_init"), c(typeof e.emscripten_stack_get_free < "u", "missing Wasm export: emscripten_stack_get_free"), c(typeof e._emscripten_stack_restore < "u", "missing Wasm export: _emscripten_stack_restore"), c(typeof e._emscripten_stack_alloc < "u", "missing Wasm export: _emscripten_stack_alloc"), c(typeof e.emscripten_stack_get_current < "u", "missing Wasm export: emscripten_stack_get_current"), c(typeof e.memory < "u", "missing Wasm export: memory"), c(typeof e.__indirect_function_table < "u", "missing Wasm export: __indirect_function_table"), xe = I("__getTypeName", 1), a._add = I("add", 2), a._multiply = I("multiply", 2), a.__Z5greetRKNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE = I("_Z5greetRKNSt3__212basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEE", 2), ae = I("malloc", 1), oe = e.emscripten_stack_get_end, e.emscripten_stack_get_base, A = I("free", 1), $e = e.emscripten_stack_init, e.emscripten_stack_get_free, Be = e._emscripten_stack_restore, Ve = e._emscripten_stack_alloc, He = e.emscripten_stack_get_current, Y = e.memory, se = e.__indirect_function_table;
  }
  var Ge = { _abort_js: fr, _embind_register_bigint: pr, _embind_register_bool: mr, _embind_register_emval: yr, _embind_register_float: Sr, _embind_register_function: $r, _embind_register_integer: Br, _embind_register_memory_view: Vr, _embind_register_std_string: Gr, _embind_register_std_wstring: Zr, _embind_register_void: Xr, emscripten_resize_heap: tt, fd_close: nt, fd_seek: it, fd_write: st }, je;
  function mt() {
    $e(), Ye();
  }
  function vt() {
    mt(), Xe();
    function e() {
      var _a2;
      c(!je), je = true, a.calledRun = true, !re && (Qe(), _e == null ? void 0 : _e(a), (_a2 = a.onRuntimeInitialized) == null ? void 0 : _a2.call(a), B("onRuntimeInitialized"), c(!a._main, 'compiled without a main, but one is present. if you added it from JS, use Module["onRuntimeInitialized"]'), er());
    }
    a.setStatus ? (a.setStatus("Running..."), setTimeout(() => {
      setTimeout(() => a.setStatus(""), 1), e();
    }, 1)) : e(), te();
  }
  var M;
  M = await sr(), vt(), j ? K = a : K = new Promise((e, r) => {
    _e = e, ge = r;
  });
  for (const e of Object.keys(a)) e in q || Object.defineProperty(q, e, { configurable: true, get() {
    h(`Access to module property ('${e}') is no longer possible via the module constructor argument; Instead, use the result of the module constructor.`);
  } });
  return K;
}
const Ft = Object.freeze(Object.defineProperty({ __proto__: null }, Symbol.toStringTag, { value: "Module" }));
export {
  wt as default
};
