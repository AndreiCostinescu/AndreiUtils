// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 27.08.21.
//

#ifndef ANDREIUTILS_NETWORKDATA_H
#define ANDREIUTILS_NETWORKDATA_H

namespace AndreiUtils {
    class NetworkData {
    public:
        static int bigEndian;

        static bool isBigEndian();

        static bool isLittleEndian();

        static void shortToNetworkBytes(char *buffer, int start, short value);

        static short networkBytesToShort(const char *buffer, int start);

        static void intToNetworkBytes(char *buffer, int start, int value);

        static int networkBytesToInt(const char *buffer, int start);

        static void longLongToNetworkBytes(char *buffer, int start, long long value);

        static long long networkBytesToLongLong(const char *buffer, int start);

        static void floatToNetworkBytes(char *buffer, int start, float value);

        static float networkBytesToFloat(const char *buffer, int start);

        static void doubleToNetworkBytes(char *buffer, int start, double value);

        static double networkBytesToDouble(const char *buffer, int start);
    };
} // namespace AndreiUtils

#endif // ANDREIUTILS_NETWORKDATA_H
