{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "addon.cc",
        "myobject.cc",
        "GccGeometry.cpp",
        "GccAnalysis.cpp",
        "GSizerKLib/NextBody.cpp",
        "GSizerKLib/AutoSizer.cpp",
        "GSizerKLib/measurepose1.cpp",
        "GSizerKLib/GLandMark.cpp",
        "GSizerKLib/Gelem/GVector3f.cpp",
        "GSizerKLib/Gelem/GPlane.cpp",
        "GSizerKLib/util.cpp",
        "GSizerKLib/ConvexHull.cpp",
        "GSizerKLib/GNurbs/GNurbsCrv.cpp",
        "GSizerKLib/GNurbs/GObject.cpp",
        "GSizerKLib/GNurbs/gnurbsutil.cpp",
        "GSizerKLib/GNurbs/linearsystem.cpp",
        "GSizerKLib/Gelem/GQuaternion.cpp"
      ],
      'cflags_cc': [ '-fexceptions' ]
    }
  ]
}