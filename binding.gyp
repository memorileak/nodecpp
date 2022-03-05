{
  "targets": [
    {
      "target_name": "helloworld",
      "sources": [
        "cpp/helloworld.cpp"
      ]
    }, {
      "target_name": "helloasync",
      "sources": [
        "cpp/helloasync.cpp"
      ]
    } , {
      "target_name": "polynomial",
      "sources": [
        "cpp/polynomial.cpp"
      ],
      "cflags": ["-Wall", "-std=c++11"],
      "conditions": [
        [
          'OS=="mac"', 
          {
            "xcode_settings": {
              'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
              'OTHER_LDFLAGS': ['-stdlib=libc++'],
              'MACOSX_DEPLOYMENT_TARGET': '10.7' 
            }
          }
        ]
      ]
    }, {
      "target_name": "hellonan",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "sources": [
        "cpp/hellonan.cpp"
      ]
    }, {
      "target_name": "factorizer",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "sources": [
        "cpp/factorizer.cpp",
        "cpp/pcqueue.cpp"
      ]
    }

  ]
}
